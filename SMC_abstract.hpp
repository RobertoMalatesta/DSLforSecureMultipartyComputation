#include "SMC_values.hpp"

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