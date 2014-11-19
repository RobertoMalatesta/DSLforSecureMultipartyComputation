#include "SMC_abstract.hpp"

template<typename Expr, typename... Args>
std::string wrapper(Args... args){
	return Expr::apply().value;
}


template <typename Rhs>
struct s_negate : unary_expression<s_negate<Rhs>, Rhs>
{
    template <typename Arg1>
    static constexpr decltype(auto) eval(Arg1&& arg1)
    {
        static_assert(std::is_same<decltype(arg1), Value<int>&& >::value, "Non int argument to negation!");
        Value<int> rv;
        rv.value = std::string("(") + std::string("-") + std::forward<Arg1>(arg1).value + std::string(")");
        std::cout << "Neg" << std::endl;
        return rv;
    }
};

template <typename Lhs, typename Rhs>
struct s_plus : binary_expression<s_plus<Lhs, Rhs>, Lhs, Rhs>
{
    template <typename Arg1, typename Arg2>
    static constexpr decltype(auto) eval(Arg1&& arg1, Arg2&& arg2)
    {
        //static_assert(Arg1::value_type == Arg2::value_type, "Wrong arguments 23!");
        static_assert(std::is_same<decltype(arg1), Value<int>&& >::value, "Non int first argument to plus!");
        static_assert(std::is_same<decltype(arg2), Value<int>&& >::value, "Non int second argument to plus!");
        Value<int> rv;
        rv.value = std::string("(") + std::forward<Arg1>(arg1).value + std::string("+") + std::forward<Arg2>(arg2).value + std::string(")");
        std::cout << "Plus" << std::endl;
        return rv;
    }
};

template <typename Lhs, typename Rhs>
struct s_minus : binary_expression<s_minus<Lhs, Rhs>, Lhs, Rhs>
{
    template <typename Arg1, typename Arg2>
    static constexpr decltype(auto) eval(Arg1&& arg1, Arg2&& arg2)
    {
        //static_assert(Arg1::value_type == Arg2::value_type, "Wrong arguments 23!");
        static_assert(std::is_same<decltype(arg1), Value<int>&& >::value, "Non int first argument to minus!");
        static_assert(std::is_same<decltype(arg2), Value<int>&& >::value, "Non int second argument to minus!");
        Value<int> rv;
        rv.value = std::string("(") + std::forward<Arg1>(arg1).value + std::string("-") + std::forward<Arg2>(arg2).value + std::string(")");
        return rv;
    }
};

template <typename Lhs, typename Rhs>
struct s_and : binary_expression<s_and<Lhs, Rhs>, Lhs, Rhs>
{
    template <typename Arg1, typename Arg2>
    static constexpr decltype(auto) eval(Arg1&& arg1, Arg2&& arg2)
    {
        //static_assert(Arg1::value_type == Arg2::value_type, "Wrong arguments 23!");
        //static_assert(std::is_same<decltype(arg1), Value<int>&& >::value, "Non int first argument to minus!");
        //static_assert(std::is_same<decltype(arg2), Value<int>&& >::value, "Non int second argument to minus!");
        Value<bool> rv;
        rv.value = std::string("(") + std::forward<Arg1>(arg1).value + std::string("&&") + std::forward<Arg2>(arg2).value + std::string(")");
        return rv;
    }
};

template <typename Lhs, typename Rhs>
struct s_or : binary_expression<s_or<Lhs, Rhs>, Lhs, Rhs>
{
    template <typename Arg1, typename Arg2>
    static constexpr decltype(auto) eval(Arg1&& arg1, Arg2&& arg2)
    {
        //static_assert(Arg1::value_type == Arg2::value_type, "Wrong arguments 23!");
        //static_assert(std::is_same<decltype(arg1), Value<int>&& >::value, "Non int first argument to minus!");
        //static_assert(std::is_same<decltype(arg2), Value<int>&& >::value, "Non int second argument to minus!");
        Value<bool> rv;
        rv.value = std::string("(") + std::forward<Arg1>(arg1).value + std::string("or") + std::forward<Arg2>(arg2).value + std::string(")");
        return rv;
    }
};

template <typename Lhs, typename Rhs>
struct s_equal : binary_expression<s_equal<Lhs, Rhs>, Lhs, Rhs>
{
    template <typename Arg1, typename Arg2>
    static constexpr decltype(auto) eval(Arg1&& arg1, Arg2&& arg2)
    {
        static_assert(std::is_same<decltype(arg1), decltype(arg2)>::value, "Different type arguments in equal!");
        Value<bool> rv;
        rv.value = std::string("(") + std::forward<Arg1>(arg1).value + std::string("==") + std::forward<Arg2>(arg2).value + std::string(")");
        return rv;
    }
};

template <typename Lhs, typename Rhs>
struct s_greater : binary_expression<s_greater<Lhs, Rhs>, Lhs, Rhs>
{
    template <typename Arg1, typename Arg2>
    static constexpr decltype(auto) eval(Arg1&& arg1, Arg2&& arg2)
    {
        static_assert(std::is_same<decltype(arg1), decltype(arg2)>::value, "Different type arguments in greater!");
        Value<bool> rv;
        rv.value = std::string("(") + std::forward<Arg1>(arg1).value + std::string(">") + std::forward<Arg2>(arg2).value + std::string(")");
        return rv;
    }
};

template <typename Lhs, typename Rhs>
struct s_less : binary_expression<s_less<Lhs, Rhs>, Lhs, Rhs>
{
    template <typename Arg1, typename Arg2>
    static constexpr decltype(auto) eval(Arg1&& arg1, Arg2&& arg2)
    {
        static_assert(std::is_same<decltype(arg1), decltype(arg2)>::value, "Different type arguments in greater!");
        Value<bool> rv;
        rv.value = std::string("(") + std::forward<Arg1>(arg1).value + std::string("<") + std::forward<Arg2>(arg2).value + std::string(")");
        return rv;
    }
};

template <typename Rhs>
struct s_not : unary_expression<s_not<Rhs>, Rhs>
{
    template <typename Arg1, typename Arg2>
    static constexpr decltype(auto) eval(Arg1&& arg1)
    {
        //static_assert(Arg1::value_type == Arg2::value_type, "Wrong arguments 23!");
        static_assert(std::is_same<decltype(arg1), Value<bool>&& >::value, "Non bool argument to not!");
        //static_assert(std::is_same<decltype(arg2), Value<int>&& >::value, "Non int second argument to minus!");
        Value<bool> rv;
        rv.value = std::string("(!") + std::forward<Arg1>(arg1).value + std::string(")");
        return rv;
    }
};

template <typename Lhs, typename Mhs, typename Rhs>
struct s_iff : ternary_expression<s_iff<Lhs, Mhs, Rhs>, Lhs, Mhs, Rhs>
{
    template <typename Arg1, typename Arg2, typename Arg3>
    static constexpr decltype(auto) eval(Arg1&& arg1, Arg2&& arg2, Arg3&& arg3)
    {
        static_assert(std::is_same<decltype(arg2), decltype(arg3)>::value, "Second and third arguments in if are not of the same type!");
        //static_assert(false, theInputs);
        //TODO CHECK
        Value<int> rv;
        rv.value = std::string("(") + std::forward<Arg1>(arg1).value + std::string("?") + std::forward<Arg2>(arg2).value + std::string(":") + std::forward<Arg3>(arg3).value + std::string(")");
        std::cout << "Iff" << std::endl;
        return rv;
    }
};

