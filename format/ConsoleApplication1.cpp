#include <iostream>
#include <boost/format.hpp>

// C++20提供了source_location，其中存储了full file name, function name, cloumne, row

template<typename... Arguments>
void format_vargs(std::source_location location, std::string const& fmt, Arguments&&... args)
{
        // 注意　这个函数应该放在ｔｒｙ－ｃａｔｃｈ
        boost::format f(fmt);

        int unroll[]{ 0, (f % std::forward<Arguments>(args), 0)... };
        static_cast<void>(unroll);

        std::cout << location.file_name() << "  " << location.function_name() << "  " << boost::str(f) << std::endl;
}


#define LOG_INFO(fmt, ...) format_vargs(std::source_location::current(), fmt, __VA_ARGS__)

void main()
{
        LOG_INFO("%s %d %d", "Test", 1, 2);
}