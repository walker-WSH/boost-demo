// test1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

//int main()
//{
//	boost::asio::io_service io;
//	boost::asio::deadline_timer timer(io, boost::posix_time::seconds(5));
//	timer.wait();
//	std::cout << "after 5 seconds ! \n";
//	return 0;
//}
/************************************************************************/
/*                                                                      */
/************************************************************************/
//void print(const boost::system::error_code& /*e*/)
//{
//	std::cout << "Hello, world!\n";
//}
//
//int main()
//{
//	boost::asio::io_service io;
//	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
//	t.async_wait(print);
//	io.run();
//	std::cout<<"hehe \n";
//	return 0;
//}

/************************************************************************/
/*                                                                      */
/************************************************************************/


//void print(const boost::system::error_code&,boost::asio::deadline_timer* t, int* count)
//{
//	if (*count < 5)
//	{
//		std::cout << *count << "\n";
//		++(*count);
//
//		t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
//		t->async_wait(boost::bind(print,boost::asio::placeholders::error, t, count));
//	}
//}
//
//int main()
//{
//	boost::asio::io_service io;
//
//	int count = 0;
//	boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));
//	t.async_wait(boost::bind(print,boost::asio::placeholders::error, &t, &count));
//	io.run();
//	std::cout << "Final count is " << count << "\n";
//	return 0;
//}
/************************************************************************/
/*                                                                      */
/************************************************************************/

class printer
{
public:
	printer(boost::asio::io_service& io) : timer_(io, boost::posix_time::seconds(1)),count_(0)
	{
		timer_.async_wait(boost::bind(&printer::print, this));
	}
	~printer()
	{
		std::cout << "Final count is " << count_ << "\n";
	}
	void print()
	{
		if (count_ < 5)
		{
			std::cout << count_ << "\n";
			++count_;
			timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
			timer_.async_wait(boost::bind(&printer::print, this));
		}
	}
private:
	boost::asio::deadline_timer timer_;
	int count_;
};

int main()
{
	boost::asio::io_service io;
	printer p(io);
	io.run();

	return 0;
}
