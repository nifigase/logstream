
#include "logstream/logstream.hpp"

#ifndef LOGGING_HPP_
#define LOGGING_HPP_

static logstream::logger<> g_logger;
static logstream::level::value g_log_threshold;

void init_logging();

#define L_(lvl) LOGSTREAM_LOG(lvl, g_log_threshold, g_logger)

#endif /* LOGGING_HPP_ */
