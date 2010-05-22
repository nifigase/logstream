
#include "boost/thread/mutex.hpp"

#ifndef LOGSTREAM_THREADING_HPP_
#define LOGSTREAM_THREADING_HPP_

namespace logstream {

/**
 * Threading policies for logger
 */
template <typename ThreadingPolicy>
struct ThreadingPolicyConcept {
	void constraints() {
		p.lock();
		p.unlock();
	}
	ThreadingPolicy p;
};

class single_threading_policy {
public:
	inline void lock() {}
	inline void unlock() {}
};

class multi_threading_policy {
public:
	void lock() {
		monitor_.lock();
	}
	void unlock() {
		monitor_.unlock();
	}
private:
	boost::mutex monitor_;
};

}
#endif /* THREADING_HPP_ */
