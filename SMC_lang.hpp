#include "SMC_abstract.hpp"

/*
 * Metafunctions definition
 */

template <typename Cond, typename Then, typename Else>
struct If {} ;

template <typename Expr1, typename Expr2>
struct Plus {} ;

template <typename Expr1, typename Expr2>
struct Seq {} ;

template <typename Expr1, typename Expr2>
struct Set {} ;

template <typename Expr1>
struct Ret {} ;

template <typename type, unsigned int loop, typename Expr>
struct For{};
/* 
 * Metafunctions implementation
 */


// Evaluate plus:
template <typename Expr1, typename Expr2, typename Env>
struct Eval<Plus<Expr1,Expr2>, Env> {
    static constexpr decltype(auto) result()
    {

        auto rv0 = Eval<Expr1,Env>::result();
        auto rv1 = Eval<Expr2,Env>::result();
        static_assert(std::is_same<decltype(rv0), Value<int> >::value, "First argument of plus is non Int!");
        static_assert(std::is_same<decltype(rv1), Value<int> >::value, "Second argument of plus is non Int!");
        Value<int> rv;
        rv.value = std::string("(") + rv0.value + std::string("+") + rv1.value + std::string(")");
        return rv;
    }
};

// Evaluate the set inside seq:
template <typename T, int uId, typename Expr1, typename Expr2, typename Env>
struct Eval<Seq< Set<sharedSMCvalue<T,uId>, Expr1> ,Expr2>, Env> {
    static constexpr decltype(auto) result()
    {


        typename checkAndAdd<Binding<sharedSMCvalue<T,uId> , uId, Env>, EnvLookup<sharedSMCvalue<T,uId>, Env>::result>::result typedef newEnv;

        auto rv0 = Eval<Expr1,Env>::result();
        auto rv1 = Eval<Expr2,newEnv>::result();

        static_assert(std::is_same<decltype(rv0), Value<int> >::value || std::is_same<decltype(rv0), Value<bool> >::value, "Set must have Int or Bool after =!");
        //static_assert(std::is_same<decltype(rv1), Value<char> >::value, "Second argument of seq must be a command!");

        Value<char> rv;
        rv.value = std::string("{sh") + std::to_string(uId) + std::string("=") + rv0.value + std::string("};{") + rv1.value + std::string("}");
        return rv;
    }
};

// Evaluate the set inside seq:
template <typename T, int uId, typename Expr1, typename Expr2, typename Env>
struct Eval<Seq< Set<SMCvalue<T,uId>, Expr1> ,Expr2>, Env> {
    static constexpr decltype(auto) result()
    {
        //We must trick the compiler to evaluate the assert only when the template is used
        static_assert(!std::is_same<T,T>::value, "Set must have shared value before =!");
        Value<char> rv;
        rv.value = std::string("---");
        return rv;
    }
};

// Evaluate seq:
template <typename Expr1, typename Expr2, typename Env>
struct Eval<Seq<Expr1,Expr2>, Env> {
    static constexpr decltype(auto) result()
    {
        auto rv0 = Eval<Expr1,Env>::result();
        auto rv1 = Eval<Expr2,Env>::result();
        //static_assert(std::is_same<decltype(rv0), Value<char> >::value, "First argument of seq must be a command!");
        //static_assert(std::is_same<decltype(rv1), Value<char> >::value, "Second argument of seq must be a command!");
        Value<char> rv;
        rv.value = std::string("{") + rv0.value + std::string("};{") + rv1.value + std::string("}");
        return rv;
    }
};

// Evaluate the if:
template <typename Cond, typename Then, typename Else, typename Env>
struct Eval<If<Cond,Then,Else>,Env> {static constexpr decltype(auto) result()
    {
        auto rv0 = Eval<Cond,Env>::result();
        auto rv1 = Eval<Then,Env>::result();
        auto rv2 = Eval<Else,Env>::result();
        static_assert(!std::is_same<decltype(rv0), Value<char> >::value, "Char argument at condition of IF!");
        static_assert(std::is_same<decltype(rv1), decltype(rv2) >::value, "Both branches of if must be of the same type!");
        Value<int> rv;
        rv.value = std::string("[") + rv0.value + std::string("?") + rv1.value
             +  std::string(":") + rv2.value + std::string("]");
        return rv;
    }
};


// Evaluate ret:
template <typename Expr1, typename Env>
struct Eval<Ret<Expr1>, Env> {
    static constexpr decltype(auto) result()
    {
        auto rv0 = Eval<Expr1,Env>::result();
        static_assert(!std::is_same<decltype(rv0), Value<char> >::value, "Return argument must be an expression!");
        Value<char> rv;
        rv.value = std::string("r->") + rv0.value;
        return rv;
    }
};

template <typename type, unsigned int loop, typename Expr, typename Env>
struct Eval<For<type,loop,Expr>, Env> {
    static constexpr decltype(auto) result()
    {
        Value<char> rv;
        rv.value = std::string("<");
        for(unsigned int i=0; i<loop; i++){
            auto rv0 = Eval<Expr,Env>::result();
            //static_assert(!std::is_same<decltype(rv0), Value<char> >::value, "Return argument must be an expression!");
            rv.value += std::string("l") + std::to_string(i) + std::string("l") + rv0.value;
        }
        rv.value += std::string(">");
        return rv;
    }
};