
#include "boost/shared_ptr.hpp"
#include "boost/filesystem.hpp"
#include "boost/filesystem/fstream.hpp"

#ifndef LOGSTREAM_APPENDERS_HPP
#define LOGSTREAM_APPENDERS_HPP

namespace logstream { namespace appenders {

class appender {
public:

	virtual void append(const std::string& message) = 0;

	virtual ~appender() {}
};

template <typename StreamT>
class stream_appender : public appender {
public:
	stream_appender(StreamT& stream) : stream_(stream) {}

	void append(const std::string & message) {
		stream_ << message;
	}
private:
	StreamT& stream_;
};

template <typename StreamT>
boost::shared_ptr<appender>
make_stream_appender(StreamT& stream) {
	return boost::shared_ptr<appender>( static_cast<appender*>(new stream_appender<StreamT>(stream)) );
}

class file_appender : public appender {
public:
	file_appender(std::string& filename) : stream_(filename) {}
	file_appender(const char* filename) : stream_(std::string(filename)) {}

	~file_appender() {
		stream_.close();
	}

	void append(const std::string & message) {
		stream_ << message;
	}
private:
	boost::filesystem::ofstream stream_;
};

}}
#endif
