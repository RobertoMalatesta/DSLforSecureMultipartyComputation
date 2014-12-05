#include "SMC_abstract.hpp"
/*
 * Metafunctions definition
 */

template <typename Cond, typename Then, typename Else>
struct If {
        static const bool hasReturn = false;
} ;

template <typename Expr1, typename Expr2>
struct Plus {
        static const bool hasReturn = false;
} ;

template <typename Expr1, typename Expr2>
struct Greater {} ;

template <typename Expr1, typename Expr2>
struct Seq {
        static const bool hasReturn = false;
} ;

template <typename Expr1, typename Expr2>
struct Set {
        static const bool hasReturn = false;
} ;

template <typename Expr1>
struct Ret {
        static const bool hasReturn = false;
} ;

template <typename Expr1>
struct Id {
        static const bool hasReturn = false;
} ;

template <unsigned int loop, typename loopVar, typename Expr>
struct For{
        static const bool hasReturn = false;
};
/* 
 * Metafunctions implementation
 */

/*
 * Plus
 */
template <typename Expr1, typename Expr2, typename Env, bool IsReturnLegal>
struct Eval<Plus<Expr1,Expr2>, Env, IsReturnLegal> {
    static const bool hasReturn = false;
    static constexpr decltype(auto) result()
    {

        auto rv0 = Eval<Expr1,Env,false>::result();
        auto rv1 = Eval<Expr2,Env,false>::result();

        static_assert(std::is_same<decltype(rv0), Value<int> >::value, "First argument of plus is non Int!");
        static_assert(std::is_same<decltype(rv1), Value<int> >::value, "Second argument of plus is non Int!");
        Value<int> rv;
        rv.value = std::string("(") + rv0.value + std::string("+") + rv1.value + std::string(")");
        return rv;
    }
};

/*
 * Greater
 */
template <typename Expr1, typename Expr2, typename Env, bool IsReturnLegal>
struct Eval<Greater<Expr1,Expr2>, Env, IsReturnLegal> {
    static const bool hasReturn = false;
    static constexpr decltype(auto) result()
    {

        auto rv0 = Eval<Expr1,Env,false>::result();
        auto rv1 = Eval<Expr2,Env,false>::result();

        static_assert(std::is_same<decltype(rv0), Value<int> >::value, "First argument of greater is non Int!");
        static_assert(std::is_same<decltype(rv1), Value<int> >::value, "Second argument of greater is non Int!");
        Value<int> rv;
        rv.value = std::string("(") + rv0.value + std::string(">") + rv1.value + std::string(")");
        return rv;
    }
};

/*
 * Sequence with a set inside
 */
template <typename T, int uId, typename Expr1, typename Expr2, typename Env, bool IsReturnLegal>
struct Eval<Seq< Set<sharedSMCvalue<T,uId>, Expr1> ,Expr2>, Env, IsReturnLegal> {
    typename checkAndAdd<Binding<sharedSMCvalue<T,uId> , uId, Env>, EnvLookup<sharedSMCvalue<T,uId>, Env>::result, false>::result typedef newEnv;
    static const bool hasReturn = Eval<Expr2,newEnv,true>::hasReturn;
    static constexpr decltype(auto) result()
    {
        auto rv0 = Eval<Expr1,Env,false>::result();
        auto rv1 = Eval<Expr2,newEnv,true>::result();

        static_assert(std::is_same<decltype(rv0), Value<int> >::value || std::is_same<decltype(rv0), Value<bool> >::value, "Set must have Int or Bool after =!");
        //static_assert(std::is_same<decltype(rv1), Value<char> >::value, "Second argument of seq must be a command!");

        Value<char> rv;
        rv.value = std::string("{sh") + std::to_string(uId) + std::string("=") + rv0.value + std::string("};{") + rv1.value + std::string("}");
        return rv;
    }
};

/*
 * Sequence the trivial case
 */
template <typename Expr1, typename Expr2, typename Env, bool IsReturnLegal>
struct Eval<Seq<Expr1,Expr2>, Env, IsReturnLegal> {
    static const bool hasReturn = Eval<Expr1,Env,true>::hasReturn || Eval<Expr2,Env,true>::hasReturn;
    static constexpr decltype(auto) result()
    {
        auto rv0 = Eval<Expr1,Env,true>::result();
        auto rv1 = Eval<Expr2,Env,true>::result();
        //static_assert(std::is_same<decltype(rv0), Value<char> >::value, "First argument of seq must be a command!");
        //static_assert(std::is_same<decltype(rv1), Value<char> >::value, "Second argument of seq must be a command!");
        Value<char> rv;
        rv.value = std::string("{") + rv0.value + std::string("};{") + rv1.value + std::string("}");
        return rv;
    }
};

/*
 * If
 */
template <typename Cond, typename Then, typename Else, typename Env, bool IsReturnLegal>
struct Eval<If<Cond,Then,Else>,Env,IsReturnLegal> {
    static const bool hasReturn = Eval<Then,Env,true>::hasReturn && Eval<Else,Env,true>::hasReturn;
    static constexpr decltype(auto) result()
    {
        auto rv0 = Eval<Cond,Env,false>::result();
        auto rv1 = Eval<Then,Env,true>::result();
        auto rv2 = Eval<Else,Env,true>::result();

        static_assert(!std::is_same<decltype(rv0), Value<char> >::value, "Char argument at condition of IF!");
        static_assert(std::is_same<decltype(rv1), decltype(rv2) >::value, "Both branches of if must be of the same type!");
        Value<int> rv;
        rv.value = std::string("[") + rv0.value + std::string("?") + rv1.value
             +  std::string(":") + rv2.value + std::string("]");
        return rv;
    }
};

/*
 * Return a value (user can specify what value to return)
 */
template <typename Expr1, typename Env>
struct Eval<Ret<Expr1>, Env, true> {
    static const bool hasReturn = true;
    static constexpr decltype(auto) result()
    {
        auto rv0 = Eval<Expr1,Env,false>::result();
        static_assert(!std::is_same<decltype(rv0), Value<char> >::value, "Return argument must be an expression!");
        Value<char> rv;
        rv.value = std::string("r->") + rv0.value;
        return rv;
    }
};

/*
 * Return the id of a user
 */
template <typename T, int uId, typename Env>
struct Eval<Ret<Id<SMCvalue<T,uId> > >, Env, true> {
    static const bool hasReturn = true;
    static constexpr decltype(auto) result()
    {
        auto rv0 = Eval<SMCvalue<T,uId>,Env,false>::result();
        Value<char> rv;
        rv.value = std::string("r_id->") + rv0.value;
        return rv;
    }
};

/*
 * For loop
 */
template <unsigned int loop, typename loopVar,  typename Expr, typename Env, bool IsReturnLegal>
struct Eval<For<loop,loopVar,Expr>, Env, IsReturnLegal> {
    typename checkAndAdd<Binding<loopVar, loopVar::id, Env>, EnvLookup<loopVar, Env>::result, false>::result typedef newEnv;
    static const bool hasReturn = Eval<Expr,newEnv,true>::hasReturn;
    static constexpr decltype(auto) result()
    {
        static_assert(loop == newEnv::nonSharedValues, "Loop invariant bigger or smaller than the number of parties!");

        Value<char> rv;
        rv.value = std::string("<");
        auto rv0 = Eval<Expr,newEnv,true>::result();
        static_assert(std::is_same<decltype(rv0), Value<char> >::value, "Function expr must be a command!");
        for(unsigned int i=0; i<loop; i++){
            rv.value += std::string("l") + std::to_string(i) + std::string("l") + rv0.value;
        }
        rv.value += std::string(">");
        return rv;
    }
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Error cases                                                                                                                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Return with id but without a SMCvalue after
 */
template <typename Expr1, typename Env>
struct Eval<Ret<Id<Expr1> >, Env, true> {
    static const bool hasReturn = true;
    static constexpr decltype(auto) result()
    {
        auto rv0 = Eval<Expr1,Env,false>::result();
        static_assert(!std::is_same<Expr1,Expr1>::value, "After Id there must be an SMVvalue!");
        Value<char> rv;
        rv.value = std::string("----");
        return rv;
    }
};

/*
 * Id not inside a return
 */
template <typename Expr1, typename Env, bool IsReturnLegal>
struct Eval<Id<Expr1>, Env, IsReturnLegal> {
    static const bool hasReturn = true;
    static constexpr decltype(auto) result()
    {
        auto rv0 = Eval<Expr1,Env,false>::result();
        static_assert(!std::is_same<Expr1,Expr1>::value, "Invalid use of ID!");
        Value<char> rv;
        rv.value = std::string("----");
        return rv;
    }
};

/*
 * Return in invalid context
 */
template <typename Expr1, typename Env>
struct Eval<Ret<Expr1>, Env, false> {
    static const bool hasReturn = true;
    static constexpr decltype(auto) result()
    {
        auto rv0 = Eval<Expr1,Env,false>::result();
        static_assert(!std::is_same<Expr1,Expr1>::value, "Illegal place for return!");
        Value<char> rv;
        rv.value = std::string("---");
        return rv;
    }
};

/*
 * Trying to set something to an SMCvalue
 */
template <typename T, int uId, typename Expr1, typename Expr2, typename Env, bool IsReturnLegal>
struct Eval<Seq< Set<SMCvalue<T,uId>, Expr1> ,Expr2>, Env, IsReturnLegal> {
    static const bool hasReturn = false;
    static constexpr decltype(auto) result()
    {
        //We must trick the compiler to evaluate the assert only when the template is used
        static_assert(!std::is_same<T,T>::value, "Set can only change shared values not SMCvalues!");
        Value<char> rv;
        rv.value = std::string("---");
        return rv;
    }
};


