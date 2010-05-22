
#include "logger.hpp"

#ifndef LOGSTREAM_SCOPED_STATEMENT_HPP
#define LOGSTREAM_SCOPED_STATEMENT_HPP

namespace logstream {

/**
 * implements scoped logging concept.
 */
template <typename ThreadingPolicy>
class scoped_statement {
public:
	scoped_statement(level::value lvl, logger<ThreadingPolicy>& logger) :
		lvl_(lvl), logger_(logger) {}
	
	scoped_statement(scoped_statement& rhs) :
		lvl_(rhs.lvl_), logger_(rhs.logger_), buffer_(rhs.buffer_.str()) {}

	~scoped_statement() throw() {
		try {
			std::string s = buffer_.str();
			if (!s.empty()) {
				logger_.log(lvl_, s);
			}
		} catch(...) {}
	}


	template <typename T>
	scoped_statement& operator<< (const T& t) {
		buffer_ << t;
		return *this;
	}

private:
	level::value lvl_;
	logger<ThreadingPolicy>& logger_;
	std::stringstream buffer_;
};


template <typename ThreadingPolicy>
scoped_statement<ThreadingPolicy> 
make_scoped_statement(level::value lvl, logger<ThreadingPolicy>& logger) {
	scoped_statement<ThreadingPolicy> ss(lvl, logger);
	return ss;
}

}
#endif

