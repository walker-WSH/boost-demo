#include <cstdlib>
#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "chat_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
	chat_client(boost::asio::io_service& io_service, char* pIP,int port) : io_service_(io_service),socket_(io_service_)
	{
		m_bConnected = false;
		tcp::endpoint EndPoint(boost::asio::ip::address_v4::from_string(pIP), port);
		//async_connect�ص�����֮ǰû�з���֪���Ƿ��Ѿ����ӳɹ� ʵ�ʴ��뵱��һ�������һ�������Ա�ʾ���׽����Ƿ��Ѿ�����������.
		socket_.async_connect(EndPoint,boost::bind(&chat_client::handle_connect, this,boost::asio::placeholders::error));
	}		

	void write(const chat_message& msg)
	{
		io_service_.post(boost::bind(&chat_client::do_write, this, msg));
	}

	void close()
	{
		io_service_.post(boost::bind(&chat_client::do_close, this));
	}

private:

	void handle_connect(const boost::system::error_code& error)
	{
		if (!error)
		{
			m_bConnected = true;
			std::cout<<"���ӳɹ�"<<std::endl;
			boost::asio::async_read(socket_,
									boost::asio::buffer(read_msg_.data(), chat_message::header_length),
									boost::bind(&chat_client::handle_read_header, this,boost::asio::placeholders::error));
		}
		else
		{
			std::cout<<"����ʧ��"<<std::endl;
		}
	}

	void handle_read_header(const boost::system::error_code& error)
	{
		if (!error && read_msg_.decode_header())
		{
			boost::asio::async_read(socket_,
									boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
									boost::bind(&chat_client::handle_read_body, this,boost::asio::placeholders::error));
		}
		else
		{
			do_close();
		}
	}

	void handle_read_body(const boost::system::error_code& error)
	{
		if (!error)
		{
			std::cout << "recieve:";
			std::cout.write(read_msg_.body(), read_msg_.body_length());
			std::cout << "\n";

			boost::asio::async_read(socket_,
									boost::asio::buffer(read_msg_.data(), chat_message::header_length),
									boost::bind(&chat_client::handle_read_header, this,boost::asio::placeholders::error));
		}
		else
		{
			do_close();
		}
	}

	void do_write(chat_message msg)
	{
		if(!m_bConnected)
		{
			return;
		}
		bool write_in_progress = !write_msgs_.empty();//��ǰwrite_msgs_����Ϊ�� �Ż����async_write
		write_msgs_.push_back(msg);	//��һ����Ϣ ����Ŀǰ�ܷ��� ������У�
		if (!write_in_progress)//����async_write�� �ص�����������ǰ ��һ��������Ϣ����ȴ�
		{
			boost::asio::async_write(socket_,
									boost::asio::buffer(write_msgs_.front().data(),write_msgs_.front().length()),
									boost::bind(&chat_client::handle_write, this,boost::asio::placeholders::error));
		}
	}

	void handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			write_msgs_.pop_front();//���λص����������ú� ���ܽ�����һ��async_write
			if (!write_msgs_.empty())
			{
				boost::asio::async_write(socket_,
										boost::asio::buffer(write_msgs_.front().data(),write_msgs_.front().length()),
										boost::bind(&chat_client::handle_write, this,boost::asio::placeholders::error));
			}
		}
		else
		{
			do_close();
		}
	}

	void do_close()
	{
		socket_.close();
		m_bConnected = false;
	}

private:
	bool						m_bConnected;
	boost::asio::io_service&	io_service_;
	tcp::socket					socket_;
	chat_message				read_msg_;
	chat_message_queue			write_msgs_;
};

int main(int argc, char* argv[])
{
	boost::asio::io_service io_service;
	char* pIP = "127.0.0.1";
	int port = 8000;
	chat_client cclient(io_service, pIP,port);

	boost::thread thread(boost::bind(&boost::asio::io_service::run, &io_service));////////////////////////////////////////////////

	char line[chat_message::max_body_length + 1];
	
	while(1)
	{
		static int i = 1;
		char achMsg[200] = {0};
		sprintf(achMsg,"msgid:%d \n",i++);
		
		using namespace std; // For strlen and memcpy.
		chat_message msg;
		msg.body_length(strlen(achMsg));
		memcpy(msg.body(), achMsg, msg.body_length());
		msg.encode_header();
		cclient.write(msg);
		
		Sleep(2000);
	}

	cclient.close();
	thread.join();

	return 0;
}
