
#include <vector>

#include "boost/concept_check.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/utility.hpp"
#include "boost/foreach.hpp"

#include "appenders.hpp"
#include "format.hpp"
#include "threading.hpp"

#ifndef LOGSTREAM_LOGGER_HPP
#define LOGSTREAM_LOGGER_HPP

namespace logstream {

/**
 * @brief Main logger class. May have number of configured appenders 
 * The way it is instantiated is up to concrete application. It may be singleton, global object, etc.
 */
template <typename ThreadingPolicy = multi_threading_policy>
class logger : boost::noncopyable {
public:
	BOOST_CLASS_REQUIRE(ThreadingPolicy, logstream, ThreadingPolicyConcept);

	typedef boost::shared_ptr<appenders::appender> appender_ptr;

	/**
	 * Initially logger has 'stupid' formatter and no appenders
	 */
	logger() {}

	/**
	 * Initially logger has no appenders
	 */
	explicit logger(const format::formatter& f) : formatter_(f) {}

	void set_formatter(const format::formatter& f) {
		formatter_ = f;
	}

	void add_appender(appender_ptr app) {
		lock_guard guard(threading_);
		appenders_.push_back(app);
	}

	void log (level::value lvl, const std::string& str) {

		std::string message = formatter_.format(lvl, str);

		lock_guard guard(threading_);
		BOOST_FOREACH( appender_ptr app, appenders_ ) {
			app->append(message);
		}
	}

private:
	ThreadingPolicy threading_;

	std::vector<appender_ptr> appenders_;
	
	format::formatter formatter_;

	struct lock_guard {
		lock_guard(ThreadingPolicy& threading) : threading_(threading) {
			threading_.lock();
		}

		~lock_guard() {
			threading_.unlock();
		}
		ThreadingPolicy& threading_;
	};
};

}
#endif

