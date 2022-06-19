// deadline_timer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//#include <boost/asio.hpp>
//#include <ctime>
//#include <iostream>
//
//#if defined(WIN32)
//# include <windows.h>
//#else
//# error This example is for Windows only!
//#endif
//
//struct tick_count_traits
//{
//	class time_type
//	{
//	public:
//		time_type() : ticks_(0) {}
//	private:
//		friend struct tick_count_traits;
//		DWORD ticks_;
//	};
//
//	// The duration type.
//	class duration_type
//	{
//	public:
//		duration_type() : ticks_(0) {}
//		duration_type(DWORD ticks) : ticks_(ticks) {}
//	private:
//		friend struct tick_count_traits;
//		DWORD ticks_;
//	};
//
//	// Get the current time.
//	static time_type now()
//	{
//		time_type result;
//		result.ticks_ = ::GetTickCount();
//		return result;
//	}
//
//	// Add a duration to a time.
//	static time_type add(const time_type& t, const duration_type& d)
//	{
//		time_type result;
//		result.ticks_ = t.ticks_ + d.ticks_;
//		return result;
//	}
//
//	// Subtract one time from another.
//	static duration_type subtract(const time_type& t1, const time_type& t2)
//	{
//		return duration_type(t1.ticks_ - t2.ticks_);
//	}
//
//	// Test whether one time is less than another.
//	static bool less_than(const time_type& t1, const time_type& t2)
//	{
//		return (t2.ticks_ - t1.ticks_) < static_cast<DWORD>(1 << 31);
//	}
//
//	// Convert to POSIX duration type.
//	static boost::posix_time::time_duration to_posix_duration(const duration_type& d)
//	{
//		return boost::posix_time::milliseconds(d.ticks_);
//	}
//};
//
//typedef boost::asio::basic_deadline_timer<DWORD, tick_count_traits> tick_count_timer;
//
//void handle_timeout(const boost::system::error_code&)
//{
//	std::cout << "handle_timeout\n";
//}
//
//int main()
//{
//	try
//	{
//		boost::asio::io_service io_service;
//
//		tick_count_timer timer(io_service, 5000);
//		std::cout << "Starting synchronous wait\n";
//		timer.wait();
//		std::cout << "Finished synchronous wait\n";
//
//		timer.expires_from_now(5000);
//
//		std::cout << "Starting asynchronous wait\n";
//		timer.async_wait(handle_timeout);
//		io_service.run();
//		std::cout << "Finished asynchronous wait\n";
//	}
//	catch (std::exception& e)
//	{
//		std::cout << "Exception: " << e.what() << "\n";
//	}
//
//	return 0;
//}

/************************************************************************/
/*                                                                      */
/************************************************************************/

#include <boost/asio.hpp>
#include <ctime>
#include <iostream>

struct time_t_traits
{
	// The time type.
	typedef std::time_t time_type;

	// The duration type.
	struct duration_type
	{
		duration_type() : value(0) {}
		duration_type(std::time_t v) : value(v) {}
		std::time_t value;
	};

	// Get the current time.
	static time_type now()
	{
		return std::time(0);
	}

	// Add a duration to a time.
	static time_type add(const time_type& t, const duration_type& d)
	{
		return t + d.value;
	}

	// Subtract one time from another.
	static duration_type subtract(const time_type& t1, const time_type& t2)
	{
		return duration_type(t1 - t2);
	}

	// Test whether one time is less than another.
	static bool less_than(const time_type& t1, const time_type& t2)
	{
		return t1 < t2;
	}

	// Convert to POSIX duration type.
	static boost::posix_time::time_duration to_posix_duration(
		const duration_type& d)
	{
		return boost::posix_time::seconds(d.value);
	}
};

typedef boost::asio::basic_deadline_timer<
std::time_t, time_t_traits> time_t_timer;

void handle_timeout(const boost::system::error_code&)
{
	std::cout << "handle_timeout\n";
}

int main()
{
	try
	{
		boost::asio::io_service io_service;

		time_t_timer timer(io_service);

		timer.expires_from_now(5);
		std::cout << "Starting synchronous wait\n";
		timer.wait();
		std::cout << "Finished synchronous wait\n";

		timer.expires_from_now(5);
		std::cout << "Starting asynchronous wait\n";
		timer.async_wait(handle_timeout);
		std::cout << "???????  \n";
		io_service.run();
		std::cout << "Finished asynchronous wait\n";
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}

	return 0;
}
