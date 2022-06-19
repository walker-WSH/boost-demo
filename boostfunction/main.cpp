/*
ʹ��functionʱ������ͨ��empty��������0�Ƚ����ж����Ƿ�ָ��һ����Ч�ĺ��������functionû��ָ��һ����Ч�ĺ�����
����ʱ���׳�bad_function_call���쳣��function��clear��������ʹ�䲻�ٹ�����һ�������������������function������ǿյģ�
���øú���Ҳ��������κ�����
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
	//boost::function2< int, std::string, float > funptr1;  //��ƽ̨����

	funptr1 = &freefun;
	funptr1("float=", 10.0 );

	CFun fun;
	funptr1 = fun;
	funptr1("float=", 10.0 );

	boost::function<int(FreeClass*, std::string, float)> funptr2;
	//boost::function3< int, FreeClass*, std::string, float > funptr; //��ƽ̨�Ķ��巽��
	
	funptr2 = &FreeClass::out;//��ֵ����
	FreeClass fc;
	funptr2( &fc, "float=", 10.0 );//���÷���


	return 0;
}