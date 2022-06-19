#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
using namespace std;
using namespace boost;
using namespace boost::asio;


class FreeClass
{
public:
	int out(int num)
	{
		cout << num <<"�ص�����"<<endl;
		return 0;  
	}
};


int main()
{
	io_service iosev; // ����asio�඼��Ҫio_service���� 
	ip::tcp::socket socket(iosev); 
	ip::tcp::endpoint EndPoint(ip::address_v4::from_string("127.0.0.1"), 8000);
	system::error_code ErrorCode;
	socket.connect(EndPoint,ErrorCode);  //���ӷ����� 
		
	if(ErrorCode) // ���������ӡ������Ϣ 
	{ 
		cout<<system::system_error(ErrorCode).what()<<endl; 
		return -1; 
	} 
	
	function<int(FreeClass*,int)> funptr2;
	funptr2 = &FreeClass::out;//��ֵ���� �Դ�funptr2��Ϊ��
	while(1)
	{
		char buf[100] = {0};
		size_t len = socket.read_some(buffer(buf,100), ErrorCode); // ��������
		if(len > 0)
		{
			cout<<len<<" msg:";
			cout<<buf<<endl;
		}

		static int num = 1;
		char buf1[200] = {0};
		sprintf(buf1,"hello world %d \n",num++);
		socket.write_some(buffer(buf1,strlen(buf1)), ErrorCode); // ��ͻ��˷���hello world! 
	
		if(ErrorCode) 	// ���������ӡ������Ϣ 
		{ 
			cout<<boost::system::system_error(ErrorCode).what()<<endl;
			
			FreeClass fc;		
			funptr2( &fc,7 );//���ûص�����111
			
			bind(&FreeClass::out,&fc,_1)(8);//���ûص�����222
			
			break;
		} 
		Sleep(1000);
	}
	return 0;
}