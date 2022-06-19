#include <iostream>
#include <boost/asio.hpp>
using namespace std;
using namespace boost::asio;

int main()
{
	io_service iosev; // ����asio�඼��Ҫio_service���� 
	ip::tcp::acceptor acceptor(iosev,ip::tcp::endpoint(ip::tcp::v4(), 8000)); 
	for(;;) 
	{ 
		ip::tcp::socket socket(iosev); // socket���� 
		acceptor.accept(socket); // �ȴ�ֱ���ͻ������ӽ��� 
		
		cout<<socket.remote_endpoint().address()<<endl; // ��ʾ���ӽ����Ŀͻ��� 
		boost::system::error_code ec; 

		for(int i=0;i<4;++i)
		{
			char buf[] = "hello world!";
			socket.write_some(buffer(buf,strlen(buf)), ec); // ��ͻ��˷���hello world! 
			if(ec) 	// ���������ӡ������Ϣ 
			{ 
				cout<<boost::system::system_error(ec).what()<<endl; 
				break; 
			}
			Sleep(1000);
		}		
	} // �뵱ǰ�ͻ�������ɺ�ѭ�������ȴ���һ�ͻ����� 

	return 0;
}