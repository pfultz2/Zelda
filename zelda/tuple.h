#ifndef ZELDA_TUPLE_H
#define ZELDA_TUPLE_H


#include <tuple>
#include "requires.h"
#include "introspection.h"
//#include <boost/fusion/adapted/std_tuple.hpp>

namespace zelda{

using std::tuple;
using std::make_tuple;
using std::forward_as_tuple;
using std::tie;
//using std::tuple_cat;
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

template<class F, class T, int ...N>
auto invoke_impl(F f, T && t, seq<N...>) ZELDA_RETURNS(f(zelda::get<N>(std::forward<T>(t))...));

template<class T1, class T2, class S1, class S2>
struct tuple_cat_r_impl {};

template<class T1, class T2, int ...N1, int ...N2>
struct tuple_cat_r_impl<T1, T2, seq<N1...>, seq<N2...> >
: zelda::mpl::identity<zelda::tuple<typename zelda::tuple_element<N1, T1>::type..., typename zelda::tuple_element<N2, T2>::type...> >
{};

template<class T1, class T2>
struct tuple_cat_r
: tuple_cat_r_impl<T1, T2, typename tuple_gens<T1>::type, typename tuple_gens<T1>::type>
{};

template<class R, class T1, class T2, int ...N1, int ...N2>
R tuple_cat_impl(T1 && t1, T2 && t2, seq<N1...>, seq<N2...>)
{
    return R(std::forward<typename zelda::tuple_element<N1, T1>::type>(zelda::get<N1>(std::forward<T1>(t1)))...,
             std::forward<typename zelda::tuple_element<N2, T2>::type>(zelda::get<N2>(std::forward<T2>(t2)))...);
}

template<class T>
struct unforward_type
: zelda::mpl::identity<T> {};

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
: zelda::mpl::identity
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
}
}



template<class T>
struct is_tuple {};
//TODO: Remove explicit override and use introspection instead
template<template<class...> class Tuple, class... T>
struct is_tuple<Tuple<T...> >
: boost::mpl::bool_<true> {};


// template<class T>
// struct is_tuple
// : detail::tuple_detail::has_value_type<zelda::tuple_size<T> > {};

template<class T1, class T2>
typename detail::tuple_cat_r<T1, T2>::type
tuple_cat(T1 && t1, T2 && t2)
{
    return detail::tuple_cat_impl<typename detail::tuple_cat_r<T1, T2>::type>
    (t1, t2, typename detail::tuple_gens<T1>::type(), typename detail::tuple_gens<T2>::type());
}

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
#endif

#endif