/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    tuple.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ZELDA_TUPLE_H
#define ZELDA_GUARD_ZELDA_TUPLE_H

//#define ZELDA_NO_STD_TUPLE

#ifdef ZELDA_NO_STD_TUPLE
#include <boost/tuple/tuple.hpp>
#else
#include <tuple>
#endif

#include <zelda/pp.h>
#include <zelda/config.h>
#include <zelda/forward.h>
#include <zelda/typeof.h>
#include <zelda/result_of.h>

namespace zelda { 

#ifdef ZELDA_NO_STD_TUPLE

#define ZELDA_TUPLE_FORWARD_REF(...) __VA_ARGS__ &

using boost::tuples::tuple;
using boost::tuples::make_tuple;
using boost::tuples::tie;
using boost::tuples::get;

template<class T>
struct tuple_size
: boost::tuples::length<T> {};

template<std::size_t I, class T>
struct tuple_element
: boost::tuples::element<I, T> {};

namespace detail {

// TODO
template<class T, class Tuple>
boost::tuples::cons<T, Tuple> tuple_prepend(T& x, Tuple t)
{
    return boost::tuples::cons<T, Tuple>(x, t);
}

template<int N>
struct tuple_catter
{

    template<class Tuple1, class Tuple2>
    struct prepend_result
    {
        typedef boost::tuples::cons<typename tuple_element<N, Tuple1>::type, Tuple2> type;
    };

    template<class Tuple1, class Tuple2>
    struct cat_result
    {
        typedef typename prepend_result<Tuple1, typename tuple_catter<N-1>::template cat_result<Tuple1, Tuple2>::type>::type type;
    };

    template<class Tuple1, class Tuple2>
    typename prepend_result<Tuple1, Tuple2>::type prepend(const Tuple1& t1, const Tuple2& t2) const
    {
        return typename prepend_result<Tuple1, Tuple2>::type
        (
            get<N>(t1),
            t2
        );
    }

    template<class Tuple1, class Tuple2>
    typename cat_result<Tuple1, Tuple2>::type operator()(const Tuple1& t1, const Tuple2& t2) const
    {
        return prepend(t1, tuple_catter<N-1>()(t1, t2));
    }

};

template<>
struct tuple_catter<0>
{
    template<class Tuple1, class Tuple2>
    struct cat_result
    {
        typedef boost::tuples::cons<typename tuple_element<0, Tuple1>::type, Tuple2> type;
    };

    template<class Tuple1, class Tuple2>
    typename cat_result<Tuple1, Tuple2>::type operator()(const Tuple1& t1, const Tuple2& t2) const
    {
        return typename cat_result<Tuple1, Tuple2>::type(get<0>(t1), t2);
    }
};
}

template<class Tuple1, class Tuple2>
typename detail::tuple_catter<tuple_size<Tuple1>::value-1>::template cat_result<Tuple1, Tuple2>::type
tuple_cat(const Tuple1& t1, const Tuple2& t2)
{
    return detail::tuple_catter<tuple_size<Tuple1>::value-1>()(t1, t2);
}



#else

#define ZELDA_TUPLE_FORWARD_REF(...) __VA_ARGS__ &&

using std::tuple;
using std::make_tuple;
using std::tie;
using std::get;
using std::tuple_cat;

//tuple_size is not implemented correctly on gcc 4.6
template<class T>
struct tuple_size
: std::tuple_size<T> {};

//tuple_element is not implemented correctly on gcc 4.6
template<std::size_t I, class T>
struct tuple_element
: std::tuple_element<I, T> {};

#endif

namespace detail {
#ifdef ZELDA_NO_RVALUE_REFS

template<class T>
struct tuple_access
{
    typedef const T type;
};

template<class T>
struct tuple_access<T&>
{
    typedef T& type;
};

template<int N, class T>
struct tuple_forward_result
: tuple_access<typename tuple_element<N, T>::type>
{};

template<int N, class T>
ZELDA_FORWARD_REF(typename tuple_forward_result<N, T>::type) tuple_forward(const T& t)
{
    return zelda::forward<typename tuple_forward_result<N, T>::type>(get<N>(t));
}

#else

template<int N, class T>
struct tuple_forward_result
{
    typedef decltype(zelda::get<N>(zelda::declval<T>()))&& type;
};

template<int N>
struct tuple_invoke_forward_result
{
    template<class T>
    struct apply
    : tuple_forward_result<N, T> {};
};

template<int N, class T>
auto tuple_forward(T && t) 
ZELDA_RETURNS(zelda::forward<decltype(zelda::get<N>(zelda::forward<T>(t)))>(zelda::get<N>(std::forward<T>(t))));

#endif

}


#ifdef ZELDA_NO_VARIADIC_TEMPLATES
//
// forward_as_tuple
//
zelda::tuple<> forward_as_tuple()
{
    return zelda::tuple<>();
}
#define ZELDA_TUPLE_FORWARD_AS_TUPLE(z, n, data) \
template<ZELDA_PP_PARAMS_Z(z, n, class T)> \
zelda::tuple<ZELDA_PP_PARAMS_Z(z, n, T, ZELDA_TUPLE_FORWARD_REF() BOOST_PP_INTERCEPT)> \
forward_as_tuple(ZELDA_PP_PARAMS_Z(z, n, T, ZELDA_TUPLE_FORWARD_REF() BOOST_PP_INTERCEPT, x)) \
{ \
    return  zelda::tuple<ZELDA_PP_PARAMS_Z(z, n, T, ZELDA_TUPLE_FORWARD_REF() BOOST_PP_INTERCEPT)> \
    ( \
        ZELDA_PP_PARAMS_Z(z, n, zelda::forward<T, > BOOST_PP_INTERCEPT, (x)) \
    ); \
}
BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_PARAMS_LIMIT, ZELDA_TUPLE_FORWARD_AS_TUPLE, ~)

//
// invoke
//
namespace detail {

template<int N>
struct tuple_invoker;

#define ZELDA_TUPLE_INVOKE(z, n, data) \
template<> \
struct tuple_invoker<n> \
{ \
    template<class F, class T> \
    struct invoke_result \
    { \
        static T& t; \
        static F& f; \
        typedef ZELDA_XTYPEOF_TPL(f(ZELDA_PP_PARAMS_Z(z, n, tuple_forward<0,>(t) BOOST_PP_INTERCEPT))) type;\
    }; \
    template<class F, class T> \
    typename invoke_result<F, T>::type operator()(F f, const T& t) const \
    { \
        return f(ZELDA_PP_PARAMS_Z(z, n, tuple_forward<0,>(t) BOOST_PP_INTERCEPT)); \
    } \
};
BOOST_PP_REPEAT_1(ZELDA_PARAMS_LIMIT, ZELDA_TUPLE_INVOKE, ~)

}

template<class F, class T, int N>
struct invoke_result_impl;

#define ZELDA_TUPLE_INVOKE_RESULT(z, n, data) \
template<class F, class T> \
struct invoke_result_impl<F, T, n > \
: zelda::result_of<F(ZELDA_PP_PARAMS_Z(z, n, typename tuple_invoke_forward_result<0, >::template apply<T>::type BOOST_PP_INTERCEPT))> \
{};
BOOST_PP_REPEAT_1(ZELDA_PARAMS_LIMIT, ZELDA_TUPLE_INVOKE_RESULT, ~)

template<class F, class T>
typename detail::tuple_invoker<tuple_size<T>::value>::template invoke_result<F, T>::type
invoke(F f, const T& t)
{
    return detail::tuple_invoker<tuple_size<T>::value>()(f, t);
}

#else

//
// forward_as_tuple
//
template<class... T>
auto forward_as_tuple(T && ... x) ZELDA_RETURNS(zelda::tuple<T&&...>(zelda::forward<T>(x)...));

//
// invoke
//
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

template<class F, class T, class S>
struct invoke_result_impl;

template<class F, class T, int ...N>
struct invoke_result_impl<F, T, seq<N...> >
: zelda::result_of<F(typename tuple_forward_result<N, T>::type...)>
{};

template<class F, class T, int ...N>
auto invoke_impl(F f, T && t, seq<N...>) ZELDA_RETURNS(f(tuple_forward<N>(std::forward<T>(t))...));

}

template<class F, class T>
struct invoke_result
: detail::invoke_result_impl<F, T, typename detail::tuple_gens<T>::type> {};

template<class F, class Tuple>
auto invoke(F f, Tuple && t) ZELDA_RETURNS
(
    detail::invoke_impl(f, std::forward<Tuple>(t), typename detail::tuple_gens<Tuple>::type() ) 
);

#endif



// Decay the types
template<class T>
struct tuple_size<const T>
: zelda::tuple_size<T> {};

template<class T>
struct tuple_size<T&>
: zelda::tuple_size<T> {};

#ifndef ZELDA_NO_RVALUE_REFS
template<class T>
struct tuple_size<T&&>
: zelda::tuple_size<T> {};
#endif

template<std::size_t I, class T>
struct tuple_element<I, const T>
: zelda::tuple_element<I, T> {};

template<std::size_t I, class T>
struct tuple_element<I, T&>
: zelda::tuple_element<I, T> {};

#ifndef ZELDA_NO_RVALUE_REFS
template<std::size_t I, class T>
struct tuple_element<I, T&&>
: zelda::tuple_element<I, T> {};
#endif



}

#endif
