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
		cout << num <<"回调函数"<<endl;
		return 0;  
	}
};


int main()
{
	io_service iosev; // 所有asio类都需要io_service对象 
	ip::tcp::socket socket(iosev); 
	ip::tcp::endpoint EndPoint(ip::address_v4::from_string("127.0.0.1"), 8000);
	system::error_code ErrorCode;
	socket.connect(EndPoint,ErrorCode);  //连接服务器 
		
	if(ErrorCode) // 如果出错，打印出错信息 
	{ 
		cout<<system::system_error(ErrorCode).what()<<endl; 
		return -1; 
	} 
	
	function<int(FreeClass*,int)> funptr2;
	funptr2 = &FreeClass::out;//赋值方法 自此funptr2不为空
	while(1)
	{
		char buf[100] = {0};
		size_t len = socket.read_some(buffer(buf,100), ErrorCode); // 接收数据
		if(len > 0)
		{
			cout<<len<<" msg:";
			cout<<buf<<endl;
		}

		static int num = 1;
		char buf1[200] = {0};
		sprintf(buf1,"hello world %d \n",num++);
		socket.write_some(buffer(buf1,strlen(buf1)), ErrorCode); // 向客户端发送hello world! 
	
		if(ErrorCode) 	// 如果出错，打印出错信息 
		{ 
			cout<<boost::system::system_error(ErrorCode).what()<<endl;
			
			FreeClass fc;		
			funptr2( &fc,7 );//调用回调函数111
			
			bind(&FreeClass::out,&fc,_1)(8);//调用回调函数222
			
			break;
		} 
		Sleep(1000);
	}
	return 0;
}