/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    defer.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_DEFER_H
#define ZELDA_GUARD_FUNCTION_DEFER_H

#include <zelda/function/adaptor.h>
#include <zelda/function/is_callable.h>
#include <zelda/function/detail/nullary_tr1_result_of.h>

namespace zelda {

template<class F>
struct defer_adaptor : F
{
    defer_adaptor()
    {};

    template<class X>
    defer_adaptor(X x) : F(x)
    {}

    typedef void zelda_is_callable_by_result_tag;

    template<class X, class Enable = void>
    struct result;

    struct no_result
    {};

    template<class X>
    struct nullary_result
    {
        typedef ZELDA_XTYPEOF_TPL(zelda::declval<X>()()) type;
    };

    template<class X>
    struct result<X(), void>
    : boost::mpl::if_<is_callable<F()>, nullary_result<F>, no_result>
    {};

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
    template<class X, class T0, class... T>
    struct result<X(T0, T...), ZELDA_CLASS_REQUIRES(zelda::is_callable<F(T0, T...)>)>
    {
        typedef ZELDA_XTYPEOF_TPL(zelda::declval<F>()(zelda::declval<T0>(), zelda::declval<T>()...)) type;
    };
#else
    #define ZELDA_DEFER_ADAPTOR(z, n, data) \
    template<class X, ZELDA_PP_PARAMS_Z(z, n, class T)> \
    struct result<X(ZELDA_PP_PARAMS_Z(z, n, T)), ZELDA_CLASS_REQUIRES(zelda::is_callable<F(ZELDA_PP_PARAMS_Z(z, n, T))>)> \
    { \
        typedef ZELDA_XTYPEOF_TPL(zelda::declval<F>()(ZELDA_PP_PARAMS_Z(z, n, zelda::declval<T, >() BOOST_PP_INTERCEPT))) type; \
    };
    BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_PARAMS_LIMIT, ZELDA_DEFER_ADAPTOR, ~)

#endif

};


template<class F>
defer_adaptor<F> defer(F f)
{
    return defer_adaptor<F>(f);
}

}

ZELDA_NULLARY_TR1_RESULT_OF_N(1, zelda::defer_adaptor)


#endif
