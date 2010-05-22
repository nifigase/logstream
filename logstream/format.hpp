
#include <stdexcept>
#include <string>
#include <iosfwd>

#include "boost/thread.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/foreach.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/format.hpp"
#include "boost/date_time.hpp"

#include "logstream/level.hpp"

#ifndef LOGSTREAM_FORMAT_HPP
#define LOGSTREAM_FORMAT_HPP

namespace logstream { namespace format {

class bad_format : public std::logic_error {
public:
	bad_format(const std::string& msg) : std::logic_error(msg) {}
};

class tag_base {
public:
	virtual std::string value(level::value lvl, const std::string& str) = 0;
	virtual ~tag_base() {}
};

class tag_thread_id : public tag_base {
public:
	static const char specifier = 't';
	std::string value(level::value lvl, const std::string& str) {
		std::stringstream stream;
		stream << boost::this_thread::get_id();
		return stream.str();
	}
};

class tag_level : public tag_base {
public:
	static const char specifier = 'l';
	std::string value(level::value lvl, const std::string& str) {
		switch(lvl) {
		case level::DEBUG:
			return "DEBUG  ";
		case level::INFO:
			return "INFO   ";
		case level::WARNING:
			return "WARNING";
		case level::ERROR:
			return "ERROR  ";
		case level::FATAL:
			return "FATAL  ";
		}
		return boost::lexical_cast<std::string>(lvl);
	}
};

class tag_date : public tag_base {
public:
	static const char specifier = 'd';
	tag_date(const std::string& fmt) : fmt_(fmt) {}
	std::string value(level::value lvl, const std::string& str) {

		boost::posix_time::ptime now(boost::posix_time::second_clock::local_time());

		typedef boost::local_time::local_time_facet facet_t;
		facet_t* facet(new facet_t(fmt_.c_str()));

		std::stringstream ss;
		ss.exceptions(std::ios_base::failbit);
		ss.imbue(std::locale(ss.getloc(), facet));
		ss << now;
		return ss.str();
	}
private:
	const std::string fmt_; //@< ex: %Y-%m-%d %H:%M:%S%F%Q
};

class tag_message : public tag_base {
public:
	static const char specifier = 'm';
	std::string value(level::value lvl, const std::string& str) {
		return str;
	}
};

/**
 * @brief partially specialized consumer functors consume input stream parsing format specification for definite tags
 */
template <typename Iter, typename Tag>
struct consumer {
	std::auto_ptr<Tag> consume(Iter it, Iter end, int& pos) throw(bad_format) {
		return std::auto_ptr<Tag>(new Tag());
	}
};

template <typename Iter>
struct consumer<Iter, tag_date> {
	std::auto_ptr<tag_date> consume(Iter& it, Iter end, int& pos) throw(bad_format) {

		++it; ++pos;

		if (*it != '{') {
			std::string err("bad logging format: expected '{' on position ");
			err += pos;
			throw bad_format(err);
		}

		int tag_pos = pos - 1;

		std::string date_spec;
		date_spec.reserve(10);

		while (++it != end) {
			++pos;
			if (*it == '}') { return std::auto_ptr<tag_date>(new tag_date(date_spec)); }

			date_spec += *it;
		}
		//not found '}'
		std::string err("bad logging format: not found enclosing '}' for date specifier on position ");
		err += tag_pos;
		throw bad_format(err);
	}
};

/**
 * Represents log format.
 * Format is of form "...%t ... %l .. %m ... %d{datefmt}"
 * Where %t will be replaced to thread ID
 * 		 %l - level name
 * 		 %d{datefmt} - current date time formatted according to Fmt. We use boost:date_time formats
 * 				See http://www.boost.org/doc/libs/1_39_0/doc/html/date_time/date_time_io.html#date_time.format_flags
 * 		 %m message to log
 */
class formatter {
public:
	/**
	 * 'stupid' default formatter - prints message only
	 */
	formatter() : canonical_format_("%s\n") {
		append_tag(std::auto_ptr<tag_message>( new tag_message()));
	}

	explicit formatter(const std::string& fmt) throw(format::bad_format) {
		typedef std::string::const_iterator iter_t;

		canonical_format_.reserve(fmt.length());

		int pos = 0;
		std::string::const_iterator end = fmt.end();
		bool was_percent = false;
		for (iter_t it = fmt.begin(); it != end; ++it, ++pos) {

			if (*it == '%') {
				was_percent = !was_percent;
				canonical_format_ += '%';
				continue;
			}

			if (was_percent) {
				switch (*it) {
				case tag_thread_id::specifier:
					consumer<iter_t, tag_thread_id> c1;
					append_tag(c1.consume(it, end, pos));
					break;
				case tag_level::specifier:
					consumer<iter_t, tag_level> c2;
					append_tag(c2.consume(it, end, pos));
					break;
				case tag_date::specifier:
					consumer<iter_t, tag_date> c3;
					append_tag(c3.consume(it, end, pos));
					break;
				case tag_message::specifier:
					consumer<iter_t, tag_message> c4;
					append_tag(c4.consume(it, end, pos));
					break;
				default:
					std::stringstream err;
					err << "unknown substitution specifier '" << *it << "' on position " << pos;
					throw bad_format(err.str());
				}
				canonical_format_ += 's';
				was_percent = false;
			} else {
				canonical_format_ += *it;
			}
		}
	}

	std::string format(level::value lvl, const std::string& str) {
		boost::format f(canonical_format_);
		BOOST_FOREACH(tag_ptr ptag, tags_) {
			f % ptag->value(lvl, str);
		}
		return f.str();
	}

private:
	template <typename Tag>
	void append_tag(std::auto_ptr<Tag> tag) {
		tag_base* base = static_cast<tag_base*>(tag.release());
		tags_.push_back(tag_ptr(base));
	}

	std::string canonical_format_;

	typedef boost::shared_ptr<tag_base> tag_ptr;
	std::vector<tag_ptr> tags_;
};

}}
#endif
