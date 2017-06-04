// BoostLog.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "logger.h"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

#include <boost/log/sinks/text_file_backend.hpp>

#include <process.h>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

	
void RemoteThreadLog(void *dummy)
{
	std::cout << "Thead started." << std::endl;
	
	BOOST_LOG_FUNCTION();//Need to call this in each thread in order to get thread specific info.

	src::severity_logger_mt<sign_severity_level>& lg = my_logger::get();

	for (int i = 0; i<10000; i++)
	{
		BOOST_LOG_SEV(lg, trace) << "A trace severity message";
		BOOST_LOG_SEV(lg, debug) << "A debug severity message";
		BOOST_LOG_SEV(lg, info) << "An informational severity message";
		BOOST_LOG_SEV(lg, warning) << "A warning severity message";
		BOOST_LOG_SEV(lg, error) << "An error severity message";
		BOOST_LOG_SEV(lg, fatal) << "A fatal severity message";
		BOOST_LOG_SEV(lg, report) << "A report severity message";
	}

	std::cout << "Thead ended." << std::endl;
};



int _tmain(int argc, _TCHAR* argv[])
{
	InitLog();
	//BOOST_LOG_FUNCTION();//Need to call this in each thread in order to get thread specific info.
	//src::severity_logger_mt<sign_severity_level>& lg = my_logger::get();
	//BOOST_LOG_SEV(lg, trace) << "A trace severity message";
	//BOOST_LOG_SEV(lg, debug) << "A debug severity message";
	//BOOST_LOG_SEV(lg, info) << "An informational severity message";
	//BOOST_LOG_SEV(lg, warning) << "A warning severity message";
	//BOOST_LOG_SEV(lg, error) << "An error severity message";
	//BOOST_LOG_SEV(lg, fatal) << "A fatal severity message";
	//BOOST_LOG_SEV(lg, report) << "A report severity message";


	if (_beginthread(RemoteThreadLog, 0, NULL) == -1)
	{
		printf("Failed to remote start create a thread\n");
		return -1;
	}

	std::string mystring;
	std::cin >> mystring;

	logging::core::get()->remove_all_sinks();
	return 0;

}

