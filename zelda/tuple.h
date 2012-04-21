#ifndef ZELDA_TUPLE_H
#define ZELDA_TUPLE_H


#include <tuple>
#include <zelda/requires.h>
#include <zelda/returns.h>
#include <zelda/introspection.h>
#include <zelda/tpl/not.h>
//#include <boost/fusion/adapted/std_tuple.hpp>

namespace zelda{

using std::tuple;
using std::make_tuple;
using std::forward_as_tuple;
using std::tie;
using std::tuple_cat;
//using std::tuple_size;
using std::get;
//using std::tuple_element;

//tuple_size is not implemented correctly on gcc 4.6
template<class T>
struct tuple_size
: std::tuple_size<T> {};

template<class T>
struct tuple_size<const T>
: zelda::tuple_size<T> {};

template<class T>
struct tuple_size<T&>
: zelda::tuple_size<T> {};

template<class T>
struct tuple_size<T&&>
: zelda::tuple_size<T> {};

//tuple_element is not implemented correctly on gcc 4.6
template<std::size_t I, class T>
struct tuple_element
: std::tuple_element<I, T> {};

template<std::size_t I, class T>
struct tuple_element<I, const T>
: zelda::tuple_element<I, T> {};

template<std::size_t I, class T>
struct tuple_element<I, T&>
: zelda::tuple_element<I, T> {};

template<std::size_t I, class T>
struct tuple_element<I, T&&>
: zelda::tuple_element<I, T> {};

template<class... Tuple>
struct tuple_join
{
    typedef decltype(tuple_cat(zelda::declval<Tuple>()...)) type;
};

namespace detail {
template<int ...>
struct seq {};

template<int N, int ...S>
struct gens : gens<N-1, N-1, S...> {};

template<int ...S>
struct gens<0, S...> 
{
  typedef seq<S...> type;
};

template<class T>
struct tuple_gens
: gens<zelda::tuple_size<T>::value> {};

template<class T, class Seq>
struct tuple_rvalue_check_impl {};

template<class T, int... N>
struct tuple_rvalue_check_impl<T, seq<N...> >
: zelda::tpl::if_< zelda::tpl::or_<std::is_rvalue_reference<typename zelda::tuple_element<N, T>::type>...>,
zelda::tpl::not_<std::is_lvalue_reference<T> > 
>::template else_< boost::mpl::bool_<true> >
{};

template<class T>
struct tuple_rvalue_check
: tuple_rvalue_check_impl<T, typename tuple_gens<T>::type>
{};

template<int N, class T>
auto forward_get(T && t) 
ZELDA_RETURNS(std::forward<decltype(zelda::get<N>(std::forward<T>(t)))>(zelda::get<N>(std::forward<T>(t))));

template<class F, class T, int ...N>
auto invoke_impl(F f, T && t, seq<N...>) ZELDA_RETURNS(f(forward_get<N>(std::forward<T>(t))...));

template<class T>
struct is_empty_tuple
: boost::mpl::bool_<zelda::tuple_size<T>::value == 0> {};

template<class T1, class T2, class S1, class S2>
struct tuple_cat_r_impl {};

template<class T1, class T2, int ...N1, int ...N2>
struct tuple_cat_r_impl<T1, T2, seq<N1...>, seq<N2...> >
: zelda::tpl::identity<zelda::tuple<typename zelda::tuple_element<N1, T1>::type..., typename zelda::tuple_element<N2, T2>::type...> >
{};



template<class R, class T1, class T2, int ...N1, int ...N2>
ZELDA_FUNCTION_REQUIRES(not is_empty_tuple<T1>, not is_empty_tuple<T2>) 
(R) tuple_cat_impl(T1 && t1, T2 && t2, seq<N1...>, seq<N2...>)
{
    return R(forward_get<N1>(std::forward<T1>(t1))...,
             forward_get<N2>(std::forward<T2>(t2))...);
}

template<class R, class T1, class T2, int ...N1, int ...N2>
ZELDA_FUNCTION_REQUIRES(is_empty_tuple<T1>, not is_empty_tuple<T2>) 
(T2) tuple_cat_impl(T1 &&, T2 && t2, seq<N1...>, seq<N2...>)
{
    return std::forward<T2>(t2);
}

template<class R, class T1, class T2, int ...N1, int ...N2>
ZELDA_FUNCTION_REQUIRES(not is_empty_tuple<T1>, is_empty_tuple<T2>) 
(T1) tuple_cat_impl(T1 && t1, T2 &&, seq<N1...>, seq<N2...>)
{
    return std::forward<T1>(t1);
}

template<class T>
struct unforward_type
: zelda::tpl::identity<T> {};

template<class T>
struct unforward_type<T&&>
: unforward_type<T> {};

template<class T>
T& unforward(T& x)
{
    return x;
}

template<class T>
const T& unforward(const T& x)
{
    return x;
}

template<class T>
T unforward(T&& x)
{
    return T(std::forward<T>(x));
}

template<class T, class S>
struct unforward_tuple_type {};

template<class T, int... N>
struct unforward_tuple_type<T, seq<N...> >
: zelda::tpl::identity
< 
zelda::tuple<typename unforward_type<typename zelda::tuple_element<N, T>::type>::type...>
> {};


template<class T, int... N>
auto unforward_tuple_impl(T && t, seq<N...>) ZELDA_RETURNS
(
    typename unforward_tuple_type<T, seq<N...> >::type(zelda::get<N>(std::forward<T>(t))...)
)

namespace tuple_detail {
ZELDA_HAS_TYPE(value_type)
ZELDA_HAS_STATIC_MEMBER(value)
}
}



template<class T>
struct is_tuple 
: zelda::detail::tuple_detail::has_value<zelda::tuple_size<T> > {};


template<class T1, class T2>
struct tuple_cat_result
: zelda::tpl::if_<detail::is_empty_tuple<T1>, zelda::tpl::lazy<std::remove_reference<T2> > >
::template else_if<detail::is_empty_tuple<T2>, zelda::tpl::lazy<std::remove_reference<T1> > >
::template else_< zelda::tpl::lazy<detail::tuple_cat_r_impl<T1, T2, typename detail::tuple_gens<T1>::type, typename detail::tuple_gens<T1>::type> > >
{};

// template<class T1, class T2>
// typename tuple_cat_result<T1, T2>::type
// tuple_cat(T1 && t1, T2 && t2)
// {
//     static_assert(detail::tuple_rvalue_check<T1>::type::value, "t1 must be an rvalue");
//     static_assert(detail::tuple_rvalue_check<T2>::type::value, "t2 must be an rvalue");
//     return detail::tuple_cat_impl<typename tuple_cat_result<T1, T2>::type>
//     (std::forward<T1>(t1), std::forward<T2>(t2), typename detail::tuple_gens<T1>::type(), typename detail::tuple_gens<T2>::type());
// }

template<class F, class Tuple>
auto invoke(F f, Tuple && t) ZELDA_RETURNS
(
    detail::invoke_impl(f, std::forward<Tuple>(t), typename detail::tuple_gens<Tuple>::type() ) 
)

template<class Tuple>
auto unforward_tuple(Tuple && t) ZELDA_RETURNS
(
    detail::unforward_tuple_impl(std::forward<Tuple>(t), typename detail::tuple_gens<Tuple>::type() ) 
)

}

#ifdef ZELDA_TEST
static_assert(zelda::is_tuple<zelda::tuple<int> >::value, "is_tuple trait failed");

static_assert(zelda::tuple_size
<
    decltype(zelda::tuple_cat(zelda::tuple<>(), zelda::tuple<int>(1)))
>::value == 1, "tuple_cat failed");

static_assert(zelda::tuple_size
<
    decltype(zelda::tuple_cat(zelda::tuple<int>(1), zelda::tuple<int>(1)))
>::value == 2, "tuple_cat failed");
#endif

#endif