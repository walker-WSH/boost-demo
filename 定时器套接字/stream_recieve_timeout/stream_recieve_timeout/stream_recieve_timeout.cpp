// stream_recieve_timeout.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

using namespace boost::asio;
using boost::asio::ip::tcp;

class stream_handler
{
public:
	stream_handler(io_service& ios)
		: io_service_(ios),
		timer_(ios),
		acceptor_(ios, tcp::endpoint(tcp::v4(), 8000)),
		socket_(ios)
	{
		acceptor_.async_accept(socket_,
			boost::bind(&stream_handler::handle_accept, this,boost::asio::placeholders::error));
	}

	void handle_accept(const boost::system::error_code& err)
	{
		if (err)
		{
			std::cout << "Accept error: " << err.message() << "\n";
		}
		else
		{
			std::cout << "Successful accept\n";

			socket_.async_read_some(boost::asio::buffer(buf_, sizeof(buf_)),
				boost::bind(&stream_handler::handle_recv, this,boost::asio::placeholders::error));
			
			timer_.expires_from_now(boost::posix_time::seconds(5));
			timer_.async_wait(boost::bind(&stream_handler::close, this,boost::asio::placeholders::error));
		}
	}

	void handle_recv(const boost::system::error_code& err)
	{
		timer_.cancel();
		if (err)
		{
			std::cout << "Receive error: " << err.message() << "\n";
		}
		else
		{
			std::cout << "Successful receive\n";
		}
	}

	void close(const boost::system::error_code& err)
	{
		if(!err)
		{
			std::cout << "socket_.close() \n";
			socket_.close();
		}
	}

private:
	io_service&		io_service_;
	deadline_timer	timer_;
	tcp::acceptor	acceptor_;
	tcp::socket		socket_;
	char			buf_[1024];
};

void connect_handler()
{
	std::cout << "Successful connect\n";
}

int main()
{
	io_service ios;
	stream_handler sh(ios);
	tcp::socket s(ios);
	s.async_connect(tcp::endpoint(boost::asio::ip::address_v4::loopback(), 8000),boost::bind(connect_handler));
	ios.run();

	return 0;
}
