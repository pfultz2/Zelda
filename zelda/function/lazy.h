/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    lazy.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_LAZY_H
#define ZELDA_GUARD_FUNCTION_LAZY_H

// @begin
// lazy
// ====
// 
// Description
// -----------
// 
// The `lazy` function adaptor returns a lazy phoenix function so it can accept
// phoenix actors as parameters.
// 
// Synopsis
// --------
// 
//     template<class F>
//     lazy_adaptor<F> lazy(F f);
// 
// Example
// -------
// 
//     struct sum
//     {
//         template<class T>
//         T sum(T x, T y)
//         {
//             return x+y;
//         }
//     };
// 
//     using zelda::ph::_1;
//     using zelda::ph::_2;
//     assert(3 == lazy(sum())(1, 2)());
//     assert(3 == lazy(sum())(_1, _2)(1, 2));
//     assert(3 == lazy(sum())(_1, 2)(1));
// 
// @end

#include <zelda/function/adaptor.h>
#include <zelda/function/detail/nullary_tr1_result_of.h>
#include <boost/phoenix/function/function.hpp>
#include <boost/phoenix/core/is_actor.hpp>
#include <zelda/traits.h>

namespace zelda { 

template<class F>
struct lazy_adaptor : boost::phoenix::function<F>
{
    lazy_adaptor() {}

    template<class X>
    lazy_adaptor(X x) : boost::phoenix::function<F>(x)
    {}

    // We add a result since the phoenix::function isn't implemented correctly
    template<class X, class Enable = void>
    struct result;

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
    template<class X, class... T>
    struct result<X(T...), ZELDA_CLASS_REQUIRES(zelda::is_callable<X(T...)>)>
    : zelda::result_of<boost::phoenix::function<F>(const typename zelda::purify<T>::type&...)>
    {};
#else
    #define ZELDA_LAZY_ADAPTOR(z, n, data) \
    template<class X BOOST_PP_COMMA_IF(n) ZELDA_PP_PARAMS_Z(z, n, class T)> \
    struct result<X(ZELDA_PP_PARAMS_Z(z, n, T)), ZELDA_CLASS_REQUIRES(zelda::is_callable<X(ZELDA_PP_PARAMS_Z(z, n, T))>)> \
    : zelda::result_of<boost::phoenix::function<F>( ZELDA_PP_PARAMS_Z(z, n, const typename zelda::purify<T, >::type& BOOST_PP_INTERCEPT) )> \
    {};
    //
    BOOST_PP_REPEAT_1(ZELDA_PARAMS_LIMIT, ZELDA_LAZY_ADAPTOR, ~)
#endif
};

//lazy
//TODO: Use boost::phoenix::detail::expression::function_eval instead
template<class F>
lazy_adaptor<F> lazy(F f)
{
    return lazy_adaptor<F>(f);
}

}

ZELDA_NULLARY_TR1_RESULT_OF_N(1, zelda::lazy_adaptor)


#ifdef ZELDA_TEST
#include <zelda/test.h>
#include <zelda/function/static.h>
#include <zelda/function/placeholders.h>
#include <boost/phoenix/core/argument.hpp>

zelda::static_<zelda::lazy_adaptor<binary_class> > binary_lazy = {};

ZELDA_TEST_CASE(lazy_test)
{
    using zelda::ph::_1;
    using zelda::ph::_2;
    ZELDA_TEST_EQUAL(3, binary_lazy(1, 2)());
    ZELDA_TEST_EQUAL(3, binary_lazy(_1, _2)(1, 2));
    ZELDA_TEST_EQUAL(3, binary_lazy(_1, 2)(1));
}
#endif

#endif
