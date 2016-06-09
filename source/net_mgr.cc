#include "evl_net/detail/net_mgr.h"
#include <sstream>
#include <iostream>
#include <log4cplus/helpers/pointer.h>
#include <log4cplus/consoleappender.h>
#include <boost/random.hpp>
#include <boost/thread.hpp>

namespace evl
{
	namespace net
	{
		NetMgr::NetMgr()
		{
			//////////////////////////////////////////////////////////////////////////
			// console appender
			/* step 1: Instantiate an appender object */
			log4cplus::helpers::SharedObjectPtr<log4cplus::Appender> console_append(new log4cplus::ConsoleAppender());
			console_append->setName("console appender");

			/* step 2: Instantiate a layout object */
			std::string console_pattern = "%d{%y-%m-%d %H:%M:%S}> [evl_net:%p] %m%n";
			std::auto_ptr<log4cplus::Layout> console_layout(new log4cplus::PatternLayout(console_pattern));

			/* step 3: Attach the layout object to the appender */
			console_append->setLayout(console_layout);

			//////////////////////////////////////////////////////////////////////////
			// daily rolling file appender
			log4cplus::helpers::SharedObjectPtr<log4cplus::Appender> file_append(new log4cplus::DailyRollingFileAppender("evl_net.log", log4cplus::MINUTELY, false, 100));
			file_append->setName("daily rolling log file");

			std::string file_pattern = "%d{%y-%m-%d %H:%M:%S}> [evl_net:%p] %m%n";
			std::auto_ptr<log4cplus::Layout> file_layout(new log4cplus::PatternLayout(file_pattern));

			file_append->setLayout(file_layout);

			std::stringstream ss;
			ss << "evl_net_library_logger" << boost::this_thread::get_id();
			std::string logger_name;
			ss >> logger_name;
			ss.clear();

			// std::cout << logger_name << std::endl;
			if (!logger_.Init(logger_name.c_str()))
			{
				BOOST_ASSERT(false);
				return;
			}

			log4cplus::Logger& _logger = logger_.GetImpl();

			/* step 5: Attach the appender object to the logger  */
			_logger.addAppender(console_append);
			_logger.addAppender(file_append);

			/* step 6: Set a priority for the logger  */
			_logger.setLogLevel(log4cplus::ALL_LOG_LEVEL);
			
		}

		NetMgr::~NetMgr()
		{

		}

		NetMgr& get_instance()
		{
			return sNetMgr;
		}
	}
}
