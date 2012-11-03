/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    invoke.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_INVOKE_H
#define ZELDA_GUARD_FUNCTION_INVOKE_H

#include <zelda/pp.h>
#include <zelda/config.h>
#include <zelda/forward.h>
#include <zelda/typeof.h>
#include <zelda/function/result_of.h>
#include <zelda/requires.h>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>



namespace zelda { 

namespace detail {

template<class S>
struct decay_seq
: boost::remove_cv<typename boost::decay<S>::type>
{};

template<class S>
struct seq_size
: boost::fusion::result_of::size<typename decay_seq<S>::type>
{};

}

#ifndef ZELDA_NO_VARIADIC_TEMPLATES

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
struct sequence_gens
: gens<detail::seq_size<T>::value> {};

template<class F, class T, int ...N>
auto invoke_impl(F f, T && t, seq<N...>) ZELDA_RETURNS(f(boost::fusion::at_c<N>(zelda::forward<T>(t))...));

template<class F, class T, class S, class Enable = void>
struct invoke_result_impl
{};

template<class F, class T, int ...N>
struct invoke_result_impl<F, T, seq<N...> >
: zelda::result_of<F(typename boost::fusion::result_of::at_c<T, N>::type...)>
{};

}

template<class F, class T>
struct invoke_result
: detail::invoke_result_impl<F, typename boost::decay<T>::type, typename detail::sequence_gens<T>::type> {};

template<class F, class Sequence>
auto invoke(F f, Sequence && t) ZELDA_RETURNS
(
    detail::invoke_impl(f, std::forward<Sequence>(t), typename detail::sequence_gens<Sequence>::type() ) 
);

#else

namespace detail {

template<class Sequence>
struct invoke_element
{
    template<int N>
    struct at_c
    : boost::fusion::result_of::at_c<typename boost::decay<Sequence>::type, N>
    {};
};

template<int N>
struct invoke_impl
{};

template<class T>
const T& invoke_forward(const T& x)
{
    return x;
}

template<class T>
T& invoke_forward(T& x)
{
    return x;
}
#ifndef ZELDA_NO_RVALUE_REFS
#define ZELDA_INVOKE_M(z, n, data) invoke_forward(boost::fusion::at_c<n>(s))
#else
#define ZELDA_INVOKE_M(z, n, data) boost::fusion::at_c<n>(s)
#endif
#define ZELDA_INVOKE(z, n, data) \
template<> \
struct invoke_impl<n> \
{ \
    template<class F> struct result; \
    template<class F, class Sequence> \
    struct result<F(Sequence)> \
    : zelda::result_of<F(ZELDA_PP_PARAMS_Z(z, n, typename invoke_element<Sequence>::template at_c<0, >::type BOOST_PP_INTERCEPT))>\
    {}; \
    template<class F, class Sequence> \
    static typename result<F(const Sequence&)>::type call(F f, const Sequence& BOOST_PP_EXPR_IF(n, s)) \
    { \
        return f(BOOST_PP_ENUM_ ## z(n, ZELDA_INVOKE_M, ~)); \
    } \
};
BOOST_PP_REPEAT_1(ZELDA_PARAMS_LIMIT, ZELDA_INVOKE, ~)
}

template<class F, class Sequence>
struct invoke_result
: detail::invoke_impl<detail::seq_size<Sequence>::value >::template result<F(Sequence)>
{};

template<class F, class Sequence>
typename invoke_result<F, const Sequence&>::type invoke(F f, const Sequence& s)
{
    return detail::invoke_impl<detail::seq_size<Sequence>::value >::call(f, s);
}

#endif

}

#endif
