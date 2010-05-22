
#include <iostream>
#include "logging.hpp"

void init_logging() {
	g_logger.set_formatter(logstream::format::formatter("%d{%Y-%m-%d %H:%M:%S%F%Q} %l [%t] - %m\n"));
//	g_logger.set_formatter(logstream::format::formatter("%m\n"));
	g_logger.add_appender(logstream::logger<>::appender_ptr( logstream::appenders::make_stream_appender(std::cerr) ));
	g_logger.add_appender(logstream::logger<>::appender_ptr( new logstream::appenders::file_appender("out/test.log") ));
	g_log_threshold = logstream::level::WARNING;


}

int main() {
	init_logging();

	L_(DEBUG) << "this DEBUG will not appear " <<  " in logs";
	L_(INFO) << "this INFO will not appear " <<  " in logs";

	//direct creation
	logstream::scoped_statement<logstream::multi_threading_policy>(logstream::level::WARNING, g_logger)
		<< 44657
		<< "this ERROR will appear " <<  " in logs" << " bbba" << 6554;

	L_(WARNING) << "this WARNING will appear " <<  " in logs" << " aaaa" << 78;
	L_(ERROR) << "this ERROR will appear " <<  " in logs" << " bbba" << 567;
	L_(FATAL) << "this FATAL will appear " <<  " in logs" << " gfhgfh" << 9876;

	return 0;
}
