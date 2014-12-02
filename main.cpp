#include <iostream>
#include <utility>
#include <tuple>
#include <cstddef>
#include <sstream>
#include <typeinfo>
#include "SMC_lang.hpp"

//g++ main.cpp -std=c++14 -ftime-report -fdiagnostics-color=always

int main()
{    
    int a = 2;
    std::string a1 = "true";
    std::string a2 = "22";
    std::string a3 = "25";
    std::string a4 = "32";
    std::string a5 = "1552";

    typedef SMCvalue<int,1> s1;
    typedef SMCvalue<int,2> s2;
    typedef SMCvalue<int,3> s3;
    typedef SMCvalue<int,4> s4;
    
    typedef sharedSMCvalue<int,12> sh1;
    s1 v1;
    s2 v2;
    s3 v3;    //THIS MUST BE DIFFERENT
    s4 v4;
    std::string y = wrapper<
                        If<s1
                            ,Ret<Plus<s3,s2> >
                            ,Seq<
                                Set<sh1,s2>,
                                Ret<sh1>//For<int, 2, sh1>
                            >
                        >
                    >(v1,v2,v3,v4);
    std::cout << "All tests passed! " << y << std::endl;
}
