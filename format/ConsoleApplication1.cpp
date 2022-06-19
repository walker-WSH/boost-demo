#include <iostream>
#include <boost/format.hpp>

template<typename... Arguments>
void format_vargs(std::string const& fmt, Arguments&&... args)
{
        boost::format f(fmt);
         
        for (const auto &x : std::initializer_list(args)) {
                f % x ;
        }

        //int unroll[]{ 0, (f % std::forward<Arguments>(args), 0)... };
        //static_cast<void>(unroll);

        //std::cout << boost::str(f);
}
/*
void test_format()
{
        //------------ 1---------------
        std::cout << boost::format("writing %1%, x=%2% : %3%-th try")
                % "toto" % 40.23 % 50
                << std::endl;

        //------------ 2---------------
        {
                boost::format f("a=%1%, b=%2%, c=%3%, a=%1%");
                f % "string" % 2 % 10.0;

                std::cout << f.str() << std::endl;
        }

        //------------ 3---------------
        {
                boost::format f("a=%1%, b=%2%, c=%3%, d=%4%");
                f % "string";
                f % 2;
                f % 10.0;
                f % "wsh";

                std::cout << f.str() << std::endl;
        }
}
*/


class Example {
private:
        boost::format *f = nullptr;

        template <typename T>
        void setVector(T arg) {
                (*f) % arg;
        }

        template <typename T, typename... Args>
        void setVector(T arg, Args... args) {
                setVector(arg);
                setVector(std::forward<Args>(args)...);
        }

public:
        Example() : f() {}
        ~Example() {
                if (f) {
                        delete f;
                        f = nullptr;
                }
        }

        template <typename T>
        void callback(T arg) {
                (*f) % arg;
        }

        template<typename... Arguments>
        void format_vargs(std::string const& fmt, Arguments&&... args)
        {
                f = new boost::format(fmt);
                setVector(args...);

                std::cout << boost::str(*f) << '\n';

                delete f;
                f = nullptr;
        }
};
 

void main(void)
{
        Example a;
        a.format_vargs("This is test for %1%! num:%2%", "WSH", 789);
        a.format_vargs("Test %s! num:0X%X", "WSH", 789);
}