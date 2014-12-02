#include "SMC_values.hpp"

/*
 * Environment Definition
*/

// EmptyEnv is the empty environment.
struct EmptyEnv ;

template <typename Name, int Value, typename Env>
struct Binding {} ;

// EnvLookup<Name,Env> :: result looks up the value of Name in Env.
template <typename Name, typename Env>
struct EnvLookup {} ;

template <typename Name>
struct EnvLookup <Name,EmptyEnv> {
    static const int result = -1;
};

template <typename Name, int Value, typename Env>
struct EnvLookup <Name, Binding<Name,Value,Env> > 
{
    static const int result = Value;
};

template <typename Name, typename Name2, int Value2, typename Env>
struct EnvLookup <Name, Binding<Name2,Value2,Env> >
{
    static const int result = EnvLookup<Name,Env>::result;
} ;

/*
 * Eval metafunction
*/

template <typename Exp, typename Env>
struct Eval {} ;

// SMCvalues evaluate to ids:
template <typename T, int uId, typename Env>
struct Eval <SMCvalue<T,uId>, Env>
{
    static constexpr decltype(auto) result()
    {
        //auto result = EnvLookup<SMCvalue<T,uId>, Env>::result;
        static_assert(EnvLookup<SMCvalue<T,uId>, Env>::result >= 0, "Value not found inside Environment!");
        Value<T> rv;
        rv.value = std::to_string(uId);
        return rv;
    }
};

// sharedSMCvalues evaluate to ids:
template <typename T, int uId, typename Env>
struct Eval <sharedSMCvalue<T,uId>, Env>
{
    static constexpr decltype(auto) result()
    {
        //auto result = EnvLookup<SMCvalue<T,uId>, Env>::result;
        static_assert(EnvLookup<sharedSMCvalue<T,uId>, Env>::result >= 0, "Shared value has not been declared yet!!");
        Value<T> rv;
        rv.value = std::string("sh") + std::to_string(uId);
        return rv;
    }
};

/*
 * Automatic creation of the initial Environment
 */

template<typename... Args> 
struct createEnv{};

template<typename Arg1>
struct createEnv<Arg1>
{
    Binding<Arg1, Arg1::id, EmptyEnv> typedef result;
};

template<typename Arg1, typename... Args>
struct createEnv<Arg1, Args...>
{
    typename createEnv<Args...>::result typedef env;
    Binding<Arg1, Arg1::id, env> typedef result;
};

/*
 * Insert new value at Env if not already there
 */
template<typename Arg1, int found>
struct checkAndAdd{

};

template<typename Name, int Value, typename Env>
struct checkAndAdd<Binding<Name,Value,Env>, -1>
{
    Binding<Name,Value,Env> typedef result;
};

template<typename Name, int Value, typename Env, int found>
struct checkAndAdd<Binding<Name,Value,Env>, found>
{
    Env typedef result;
};


/*
 * Wrapper Class callable from user
 */

template<typename Expr, typename... Args>
std::string wrapper(Args... args){
    typename createEnv<Args...>::result typedef lala;
    //int v = EnvLookup <SMCvalue<int,2>, lala >::result;
    //static_assert(EnvLookup <SMCvalue<int,2>, lala >::result != -1, "Value not found into environment!");
    //std::cout << v << std::endl << std::endl;
    return Eval<Expr, lala>::result().value;
}
