#pragma once  

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>

#include <boost/log/core.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/exception/all.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <fstream>
#include <boost/log/utility/setup/console.hpp>


namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;


enum sign_severity_level {
	trace,
	debug,
	info,
	warning,
	error,
	fatal,
	report
};

void InitLog() {
	boost::shared_ptr<logging::core> core = logging::core::get();

	typedef sinks::synchronous_sink<sinks::text_file_backend> TextSink;

	std::string log_path("C:\\TAL\\DATA");

	// init file sink1
	boost::shared_ptr<sinks::text_file_backend> fileBackend = boost::make_shared<sinks::text_file_backend>(
		keywords::file_name = "sign_%Y-%m-%d.%3N.log",
		keywords::rotation_size = 10 * 1024 * 1024,
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
		keywords::min_free_space = 30 * 1024 * 1024);
	fileBackend->auto_flush(true); //Seems to have no effect on windows.
	boost::shared_ptr<TextSink> fileSink(new TextSink(fileBackend));
	fileSink->set_formatter(
		expr::format("%1% (%2%)(%3%)<%4%>: %5%")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
		% expr::attr<sign_severity_level>("Severity")
		% expr::attr<attrs::current_thread_id::value_type>("ThreadID")
		% expr::format_named_scope("Scopes", boost::log::keywords::format = "%n (%f : %l)")
		% expr::smessage
		);
	fileSink->set_filter(expr::attr<sign_severity_level>("Severity") >= trace);



	fileBackend->set_file_collector(sinks::file::make_collector(
		keywords::target = log_path,
		keywords::max_size = 1024 * 1024 * 500 //500MB max of log files
		));

	fileBackend->scan_for_files();


	core->add_sink(fileSink);


	//init sink2, CSV file that hold just reports
	boost::shared_ptr<sinks::text_file_backend> CSVbackend = boost::make_shared<sinks::text_file_backend>(
		keywords::file_name = "sign_%Y-%m-%d.csv",
		keywords::rotation_size = 10 * 1024 * 1024,
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(boost::gregorian::greg_day(1), 0, 0, 0),
		keywords::min_free_space = 30 * 1024 * 1024);
	CSVbackend->auto_flush(true);
	boost::shared_ptr<TextSink> CSVsink(new TextSink(CSVbackend));
	CSVsink->set_formatter(
		expr::format("%1%,%2%")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
		% expr::smessage
		);
	CSVsink->set_filter(expr::attr<sign_severity_level>("Severity") == report);
	core->add_sink(CSVsink);


	//Log to conslo all greater than info
	
	if (true)
	{

		logging::add_console_log
			(std::cout,
			keywords::format = ">> %Message%"
			, keywords::filter = expr::attr<sign_severity_level>("Severity") >= info

			);
	}
	
	logging::add_common_attributes();
	core->add_global_attribute("Scopes", attrs::named_scope());
}

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::severity_logger_mt<sign_severity_level>)