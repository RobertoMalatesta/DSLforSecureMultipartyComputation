#include "SMC_values.hpp"

/*
 * Environment Definition
*/

// EmptyEnv is the empty environment.
struct EmptyEnv{
    static const int nonSharedValues = 0;
};

template <typename Name, int Value, typename Env>
struct Binding {
    static const int nonSharedValues = Env::nonSharedValues;
} ;

template <typename T, int id, int Value, typename Env>
struct Binding<SMCvalue<T,id> , Value, Env> {
    static const int nonSharedValues = Env::nonSharedValues + 1;
} ;

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

template <typename Exp, typename Env, bool IsReturnLegal>
struct Eval {} ;

// SMCvalues evaluate to ids:
template <typename T, int uId, typename Env, bool IsReturnLegal>
struct Eval <SMCvalue<T,uId>, Env, IsReturnLegal>
{
    static constexpr decltype(auto) result()
    {
        static const bool hasReturn = false;
        static_assert(EnvLookup<SMCvalue<T,uId>, Env>::result >= 0, "Value not found inside Environment!");
        Value<T> rv;
        rv.value = std::to_string(uId);
        return rv;
    }
};

// sharedSMCvalues evaluate to ids:
template <typename T, int uId, typename Env, bool IsReturnLegal>
struct Eval <sharedSMCvalue<T,uId>, Env, IsReturnLegal>
{
    static constexpr decltype(auto) result()
    {
        static const bool hasReturn = false;
        static_assert(EnvLookup<sharedSMCvalue<T,uId>, Env>::result >= 0, "Shared value has not been declared yet!!");
        Value<T> rv;
        rv.value = std::string("sh") + std::to_string(uId);
        return rv;
    }
};

// forSMCvalues evaluate to for ids:
template <typename T, int uId, typename Env, bool IsReturnLegal>
struct Eval <forSMCvalue<T,uId>, Env, IsReturnLegal>
{
    static constexpr decltype(auto) result()
    {
        static const bool hasReturn = false;
        static_assert(EnvLookup<forSMCvalue<T,uId>, Env>::result >= 0, "Invalid for value value has not been declared yet!!");
        Value<T> rv;
        rv.value = std::string("fv") + std::to_string(uId);
        return rv;
    }
};

// forSMCvalues evaluate to for ids:
template <int uId, typename Env, bool IsReturnLegal>
struct Eval <idSMCvalue<uId>, Env, IsReturnLegal>
{
    static constexpr decltype(auto) result()
    {
        static const bool hasReturn = false;
        static_assert(EnvLookup<idSMCvalue<uId>, Env>::result >= 0, "Invalid for value value has not been declared yet!!");
        Value<char> rv;
        rv.value = std::string("idv") + std::to_string(uId);
        return rv;
    }
};

/*
 * Insert new value at Env if not already there
 */
template<typename Arg1, int found, bool FireError>
struct checkAndAdd{

};

template<typename Name, int Value, typename Env, bool FireError>
struct checkAndAdd<Binding<Name,Value,Env>, -1, FireError>
{
    Binding<Name,Value,Env> typedef result;
};

template<typename Name, int Value, typename Env, int found, bool FireError>
struct checkAndAdd<Binding<Name,Value,Env>, found, FireError>
{
    Env typedef result;
    static_assert(!FireError, "Duplicate function argument!");
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
    typename createEnv<Args...>::result typedef Env;
    typename checkAndAdd<Binding<Arg1 , Arg1::id, Env>, EnvLookup<Arg1, Env>::result, true>::result typedef result;
    //Binding<Arg1, Arg1::id, env> typedef result;
};



/*
 * Wrapper Class callable from user
 */

template<typename Expr, typename... Args>
std::string wrapper(Args... args){
    typename createEnv<Args...>::result typedef lala;
    
    static_assert(Eval<Expr, lala, true>::hasReturn, "There is no valid return in the end of the function!");

    return Eval<Expr, lala, true>::result().value;
}