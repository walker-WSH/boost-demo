/*
使用function时，可以通过empty函数或与0比较来判断其是否指向一个有效的函数。如果function没有指向一个有效的函数，
调用时会抛出bad_function_call的异常。function的clear函数可以使其不再关联到一个函数或函数对象，如果该function本身就是空的，
调用该函数也不会带来任何问题
*/


#include <iostream>
#include <boost/function.hpp>
using namespace std;

int freefun( std::string str, float f )
{
	std::cout << str << " : " << f << std::endl;
	return 0;
}

class CFun
{
public:
	int operator() ( std::string str, float f )
	{
		std::cout << str << " : " << f << std::endl;
		return 0;  
	}
};

class FreeClass
{
public:
	int out( std::string str, float f )
	{
		std::cout << str << " : " << f << std::endl;
		return 0;  
	}
};

int main()
{
	boost::function< int ( std::string, float ) > funptr1;
	//boost::function2< int, std::string, float > funptr1;  //跨平台定义

	funptr1 = &freefun;
	funptr1("float=", 10.0 );

	CFun fun;
	funptr1 = fun;
	funptr1("float=", 10.0 );

	boost::function<int(FreeClass*, std::string, float)> funptr2;
	//boost::function3< int, FreeClass*, std::string, float > funptr; //跨平台的定义方法
	
	funptr2 = &FreeClass::out;//赋值方法
	FreeClass fc;
	funptr2( &fc, "float=", 10.0 );//调用方法


	return 0;
}