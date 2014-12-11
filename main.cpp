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
    typedef SMCvalue<int,1> s1;
    typedef SMCvalue<int,2> s2;
    typedef SMCvalue<int,3> s3;
    typedef SMCvalue<int,4> s4;

    typedef forSMCvalue<int,1> f1;
    
    typedef sharedSMCvalue<int,1> sh1;
    typedef idSMCvalue<1> id1;
    s1 v1;
    s2 v2;
    s3 v3;    //THIS MUST BE DIFFERENT
    s4 v4;
    std::string y = wrapper<
                        Seq<Set<id1, s1>,
                            Seq<Set<sh1, s1>,
                                Seq<For<4, f1,
                                        If< Greater<f1,sh1>,
                                            Seq< Set<sh1, f1>,
                                                Set<id1, f1>
                                            >,
                                            Nope
                                        >
                                    >,
                                    Ret<id1>
                                >
                            >
                        >
                    >(v1,v2,v3,v4);
    std::cout << "All tests passed! " << y << std::endl;
    std::string x = wrapper<
                        If<Greater<s1,s2>,
                            Ret<Id<s1> >,
                            Ret<Id<s2> >
                        >
                    >(v1,v2);
    std::cout << "Who has it bigger?! " << x << std::endl;
}
