#include <iostream>
#include <boost/asio.hpp>
using namespace std;
using namespace boost::asio;

int main()
{
	io_service iosev; // 所有asio类都需要io_service对象 
	ip::tcp::acceptor acceptor(iosev,ip::tcp::endpoint(ip::tcp::v4(), 8000)); 
	for(;;) 
	{ 
		ip::tcp::socket socket(iosev); // socket对象 
		acceptor.accept(socket); // 等待直到客户端连接进来 
		
		cout<<socket.remote_endpoint().address()<<endl; // 显示连接进来的客户端 
		boost::system::error_code ec; 

		for(int i=0;i<4;++i)
		{
			char buf[] = "hello world!";
			socket.write_some(buffer(buf,strlen(buf)), ec); // 向客户端发送hello world! 
			if(ec) 	// 如果出错，打印出错信息 
			{ 
				cout<<boost::system::system_error(ec).what()<<endl; 
				break; 
			}
			Sleep(1000);
		}		
	} // 与当前客户交互完成后循环继续等待下一客户连接 

	return 0;
}