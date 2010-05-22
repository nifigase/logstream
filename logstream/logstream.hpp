/**
 * main logstream inclusion class
 */

#include "level.hpp"
#include "logger.hpp"
#include "scoped_statement.hpp"

#ifndef LOGSTREAM_HPP
#define LOGSTREAM_HPP

#define LOGSTREAM_LOG(lvl, threshold, logger_ref) \
	if ((threshold) > (logstream::level:: lvl)) ; \
	else ::logstream::make_scoped_statement((logstream::level:: lvl), (logger_ref))

#endif
