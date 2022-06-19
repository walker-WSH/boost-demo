#include <iostream>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

#define max_len 1024

class clientSession : public boost::enable_shared_from_this<clientSession>
{
public:

	clientSession(boost::asio::io_service& ioservice) : m_socket(ioservice)
	{
		memset(m_achData,0,max_len);
		m_MsgCount = 0;
	}
	tcp::socket& socket()
	{
		return m_socket;
	}
	void start()
	{
		boost::asio::async_write(m_socket,boost::asio::buffer("link successed!"),
								boost::bind(&clientSession::handle_write,shared_from_this(),boost::asio::placeholders::error));
		m_socket.async_read_some(boost::asio::buffer(m_achData,max_len),
								boost::bind(&clientSession::handle_read,shared_from_this(),boost::asio::placeholders::error));
	}

private:

	void handle_write(const boost::system::error_code& error)
	{
		if(error)
		{
			m_socket.close();
		}
	}
	void handle_read(const boost::system::error_code& error)
	{
		if(!error)
		{
			std::cout << m_achData << std::endl;

			char buf[100] = {0};
			sprintf(buf,"收到了第%d条消息",++m_MsgCount);
			boost::asio::async_write(m_socket,boost::asio::buffer(buf),
				boost::bind(&clientSession::handle_write,shared_from_this(),boost::asio::placeholders::error));
			m_socket.async_read_some(boost::asio::buffer(m_achData,max_len),
								boost::bind(&clientSession::handle_read,shared_from_this(),boost::asio::placeholders::error));
		}
		else
		{
			m_socket.close();
		}
	}
private:
	int			m_MsgCount;
	tcp::socket m_socket;
	char		m_achData[max_len];
};

typedef boost::shared_ptr<clientSession> session_ptr;

class serverApp
{
public:
	serverApp(boost::asio::io_service& ioservice, tcp::endpoint& endpoint) : m_ioservice(ioservice),acceptor_(ioservice,endpoint)
	{
		session_ptr  new_session(new clientSession(ioservice));
		acceptor_.async_accept(new_session->socket(),boost::bind(&serverApp::handle_accept,this,boost::asio::placeholders::error,new_session));
	}
private:
	void handle_accept(const boost::system::error_code& error,session_ptr& session)
	{
		if(!error)
		{
			std::cout<<session->socket().remote_endpoint().address()<<std::endl; // 显示连接进来的客户端 
			session->start();//在这就应该看出为什么要封session类了吧，每一个session对应于一个客户端连接
			
			session_ptr new_session(new clientSession(m_ioservice));
			acceptor_.async_accept(new_session->socket(),boost::bind(&serverApp::handle_accept,this,boost::asio::placeholders::error,new_session));
		}
	}
private:
	boost::asio::io_service& m_ioservice;
	tcp::acceptor acceptor_;
};


int main(int argc , char* argv[])
{
	boost::asio::io_service IoService;
	tcp::endpoint endPoint(tcp::v4(),8100);
	serverApp sa(IoService,endPoint);
	IoService.run();

	return 0;
}