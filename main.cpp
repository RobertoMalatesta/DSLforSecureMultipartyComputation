#include <iostream>
#include <utility>
#include <tuple>
#include <cstddef>
#include <sstream>
#include <typeinfo>
#include "SMC_lang.hpp"

//g++ main.cpp -std=c++14 -ftime-report -fdiagnostics-color=always




/*template<typename Expr>
struct wrapper{
    Expr tata;

    static constexpr decltype(auto) EXEC(){
        return Expr::apply();
    }
};*/

/*template<class<typename T, int id1> a1, class<typename T, int id2> a2, class<typename T, int id3>... args>
struct wrapper{
    void EXEC(){
        cout << "L = " << id1 << endl;
        wrapper<a2, args...>::EXEC();
    }
};

template<>struct wrapper<class<typename T,int id> >{
    void EXEC(){
        cout << "L = " << id << endl;
    }
};*/

int main()
{    
    // (1 + 2) + 3 = 6
    int a = 2;
    std::string a1 = "true";
    std::string a2 = "22";
    std::string a3 = "25";
    std::string a4 = "32";
    std::string a5 = "1552";

    //std::cout << iff<arg, plus<plus<arg, arg>,arg>, arg >::apply(a1, a2, a3, a4, a5) << std::endl;
    //std::cout << wrapper<iff<arg, plus<plus<arg, arg>,arg>, arg >>::EXEC() << std::endl;


    //std::cout << plus<arg, arg>::apply(std::string("false"), std::string("true"))<< std::endl;

    typedef SMCvalue<int,1> s1;
    typedef SMCvalue<int,2> s2;
    typedef SMCvalue<int,3> s3;
    typedef SMCvalue<int,4> s4;
    /*
    s1 v1;
    s2 v2;
    s3 v3;    //THIS MUST BE DIFFERENT
    s4 v4;*/
    std::cout << wrapper<s_iff<s1, s_plus<s_plus<s1, s2>,s3>, s_negate<s4> > >(a,a)<< std::endl;

    //std::cout << <plus<s1, s2>::apply().getIP() << std::endl;
    //std::cout << negate<negate<s1> >::apply().value << std::endl;
    //std::cout << SMCvalue<int, 4>::arity << std::endl;
    //wrapper<v1>::EXEC();

    // ((a + 5) + (2 * 6)) - 5 = 16
    //int a = 4;
    //std::cout << minus<plus<plus<arg, arg>, multiply<arg, constant<int, 6>>>, constant<int, 5>>::apply(a, 5, 2) << std::endl;

    // ((1 * 2) * 3) * 4 = 24
    //std::cout << multiply<multiply<multiply<arg, arg>, arg>, arg>::apply(1, 2, 3, 4) << std::endl;

    // -((3 * 4) + (5 - 6)) = -11
    //static_assert(negate<plus<multiply<arg, arg>, minus<arg, arg>>>::apply(3, 4, 5, 6) == -11, "!");
}