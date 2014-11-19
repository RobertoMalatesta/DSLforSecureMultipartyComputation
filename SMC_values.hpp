#include <iostream>
#include <utility>
#include <tuple>
#include <cstddef>
#include <sstream>
#include <typeinfo>

static bool inputs[100];
static std::string theInputs;
static int numOfInputs;

template <std::size_t Arity>
struct expression
{    
    static constexpr std::size_t arity = Arity;
};

struct arg : expression<1>
{
    template <typename Arg1>
    static constexpr decltype(auto) apply(Arg1&& arg1)
    {
        return -2;//std::forward<Arg1>(arg1);
    }
};

template <typename Type, Type value>
struct constant : expression<0>
{    
    static constexpr decltype(auto) apply()
    {
        return value;
    }
};

template<typename T>
class Value{
public:
    std::string value;
};

template<typename T, int uId>
class SMCvalue : public expression<0>{
protected:
    std::string id = uId;
    std::string smcip;
public:
    SMCvalue(){};
    SMCvalue(std::string ip):smcip(ip){};
public:
    void setIP(std::string ip){
        smcip = ip;
    }

    std::string getIP(){
        return smcip;
    }

    int getValue(){
        return uId;
    }

    /*static constexpr decltype(auto) apply()
    {
        std::stringstream ss;
        ss << uId;
        return ss.str();
    }*/
    static constexpr decltype(auto) apply()
    {
        Value<T> v;
        std::stringstream ss;
        ss << uId;
        v.value = ss.str();
        theInputs = std::string(" ") + v.value;
        std::cout << theInputs << std::endl;    
        return v;
    }

};

template<typename T, int id>
class mySMCvalue : SMCvalue<T, id>{
private:
    T myValue;
public:
    mySMCvalue():SMCvalue<T,id>(){};
    mySMCvalue(std::string ip):SMCvalue<T,id>(ip){};
    T getMyValue(){
        return myValue;
    }
    void setMyValue(T inp){
        myValue = inp;
    }
};