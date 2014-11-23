#include "SMC_values.hpp"

// EmptyEnv is the empty environment.
struct EmptyEnv ;

// Bindings<Name,Value,Env> is a type than encodes the environment Env
// extended with a binding for Name => Value.
template <typename Name, int Value, typename Env>
struct Binding {} ;

// EnvLookup<Name,Env> :: result looks up the value of Name in Env.
template <typename Name, typename Env>
struct EnvLookup {} ;

template <typename Name>
struct EnvLookup <Name,EmptyEnv> {
    //static_assert(false, "You reach the empty Env!");
    static const int result = -1;
} ; // Name not found.

template <typename Name, int Value, typename Env>
struct EnvLookup <Name, Binding<Name,Value,Env> > 
{
    static const int result = Value;
} ;

template <typename Name, typename Name2, int Value2, typename Env>
struct EnvLookup <Name, Binding<Name2,Value2,Env> >
{
    static const int result = EnvLookup<Name,Env>::result;
} ;

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


template<typename Expr, typename... Args>
std::string wrapper(Args... args){
	typename createEnv<Args...>::result typedef lala;
    int v = EnvLookup <SMCvalue<int,2>, lala >::result;
    static_assert(EnvLookup <SMCvalue<int,2>, lala >::result != -1, "Value not found into environment!");
    std::cout << v << std::endl << std::endl;
	return 2;//Expr::apply().value;
}


template <typename Expr, typename Rhs>
struct unary_expression : expression<Rhs::arity>
{    
    template <typename... Args>
    static constexpr decltype(auto) apply(Args&&... args)
    {
        static_assert(sizeof...(Args) == unary_expression::arity, "Wrong number of operands!");
        return Expr::eval(Rhs::apply(std::forward<Args>(args)...));
    }
};

template <typename Expr, typename Lhs, typename Rhs>
struct binary_expression : expression<Lhs::arity + Rhs::arity>
{
    template <typename... Args>
    static constexpr decltype(auto) apply(Args&&... args)
    {
        static_assert(sizeof...(Args) == binary_expression::arity, "Wrong number of operands!");
        return _apply(std::make_index_sequence<Lhs::arity>{}, std::make_index_sequence<Rhs::arity>{}, std::tuple<Args&&...>(std::forward<Args>(args)...));
    }

    template <typename Tuple, std::size_t... Arity1, std::size_t... Arity2>
    static constexpr decltype(auto) _apply(std::index_sequence<Arity1...>, std::index_sequence<Arity2...>, Tuple&& args)
    {
        return Expr::eval(Lhs::apply(static_cast<typename std::tuple_element<Arity1, Tuple>::type>(std::get<Arity1>(args))...),
                          Rhs::apply(static_cast<typename std::tuple_element<Lhs::arity + Arity2, Tuple>::type>(std::get<Lhs::arity + Arity2>(args))...));
    }
};

template <typename Expr, typename Lhs,typename Mhs, typename Rhs>
struct ternary_expression : expression<Lhs::arity + Rhs::arity + Mhs::arity>
{
    template <typename... Args>
    static constexpr decltype(auto) apply(Args&&... args)
    {
        static_assert(sizeof...(Args) == ternary_expression::arity, "Wrong number of operands!");
        return _apply(std::make_index_sequence<Lhs::arity>{}, std::make_index_sequence<Mhs::arity>{}, std::make_index_sequence<Rhs::arity>{}, std::tuple<Args&&...>(std::forward<Args>(args)...));
    }

    template <typename Tuple, std::size_t... Arity1, std::size_t... Arity2, std::size_t... Arity3>
    static constexpr decltype(auto) _apply(std::index_sequence<Arity1...>, std::index_sequence<Arity2...>, std::index_sequence<Arity3...>, Tuple&& args)
    {
        return Expr::eval(Lhs::apply(static_cast<typename std::tuple_element<Arity1, Tuple>::type>(std::get<Arity1>(args))...),
                          Mhs::apply(static_cast<typename std::tuple_element<Lhs::arity + Arity2, Tuple>::type>(std::get<Lhs::arity + Arity2>(args))...),
                          Rhs::apply(static_cast<typename std::tuple_element<Lhs::arity + Mhs::arity + Arity3, Tuple>::type>(std::get<Lhs::arity + Mhs::arity + Arity3>(args))...));
    }
};


