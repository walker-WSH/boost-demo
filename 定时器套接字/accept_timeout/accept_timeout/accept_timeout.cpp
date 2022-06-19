// accept_timeout.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <iostream>

using namespace boost::asio;
using boost::asio::ip::tcp;

class accept_handler
{
public:
	accept_handler(io_service& ios)
		: io_service_(ios),
		timer_(ios),
		acceptor_(ios, tcp::endpoint(tcp::v4(), 32123)),
		socket_(ios)
	{
		acceptor_.async_accept(socket_,
			boost::bind(&accept_handler::handle_accept, this,
			boost::asio::placeholders::error));

		timer_.expires_from_now(boost::posix_time::seconds(5));
		timer_.async_wait(boost::bind(&accept_handler::close, this,boost::asio::placeholders::error));
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
		}
	}

	void close(const boost::system::error_code& err)
	{
		if(0 == err)
		{
			acceptor_.close();
		}
	}

private:
	io_service& io_service_;
	deadline_timer timer_;
	tcp::acceptor acceptor_;
	tcp::socket socket_;
	boost::system::error_code m_error; 
};

int main()
{
	io_service ios;
	accept_handler ah(ios);
	ios.run();

	return 0;
}
