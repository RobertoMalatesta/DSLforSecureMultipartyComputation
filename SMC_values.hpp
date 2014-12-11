#include <iostream>
#include <utility>
#include <tuple>
#include <cstddef>
#include <sstream>
#include <typeinfo>

template<typename T>
class Value{
public:
    std::string value;
};

template<typename T, int uId>
class SMCvalue{
    static_assert(uId >= 0, "The unique ID must be greater than zero!");
protected:
    std::string smcip;
public:
    SMCvalue(){};
    SMCvalue(std::string ip):smcip(ip){};
    static const int id = uId;
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

template<typename T, int uId>
struct sharedSMCvalue{
};

template<typename T, int fId>
struct forSMCvalue{
    static const int id = fId;
};

template<int fId>
struct idSMCvalue{
    static const int id = fId;
};
