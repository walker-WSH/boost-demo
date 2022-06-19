// connect_timeout.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

using namespace boost::asio;
using boost::asio::ip::tcp;

class connect_handler
{
public:
	connect_handler(io_service& ios) : io_service_(ios),timer_(ios),socket_(ios)
	{
		socket_.async_connect(tcp::endpoint(boost::asio::ip::address_v4::loopback(), 8000),
							boost::bind(&connect_handler::handle_connect, this,boost::asio::placeholders::error));
		timer_.expires_from_now(boost::posix_time::seconds(5));
		timer_.async_wait(boost::bind(&connect_handler::close, this ,boost::asio::placeholders::error));
		//timer_.wait();
	}
	void handle_connect(const boost::system::error_code& err)
	{
		timer_.cancel();
		if (err)
		{
			std::cout << "Connect error: " << err.message() << "\n";
		}
		else
		{
			std::cout << "Successful connection\n";
		}
	}
	void close(const boost::system::error_code& err)
	{
		if(0 == err)
		{
			socket_.close();
			std::cout << "socket_.close()  \n";
		}
		else
		{
			std::cout << "error: " << err.message() << "\n";
		}
	}
private:
	io_service& io_service_;
	deadline_timer timer_;
	tcp::socket socket_;
	boost::system::error_code m_error; 
};

int main()
{
	io_service ios;
	tcp::acceptor a(ios, tcp::endpoint(tcp::v4(), 8000), 1);

	connect_handler ch1(ios);
	//connect_handler ch2(ios);

	ios.run();

	return 0;
}
