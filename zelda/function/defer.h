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

    template<class X, class Enable = void>
    struct result;

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
    template<class X, class... T>
    struct result<X(T...), ZELDA_CLASS_REQUIRES(zelda::is_callable<X(T...)>)>
    {
        typedef ZELDA_XTYPEOF_TPL(zelda::declval<X>()(zelda::declval<T>()...)) type;
    };
#else
    #define ZELDA_DEFER_ADAPTOR(z, n, data) \
    template<class X BOOST_PP_COMMA_IF(n) ZELDA_PP_PARAMS_Z(z, n, class T)> \
    struct result<X(ZELDA_PP_PARAMS_Z(z, n, T)), ZELDA_CLASS_REQUIRES(zelda::is_callable<X(ZELDA_PP_PARAMS_Z(z, n, T))>)> \
    { \
        typedef ZELDA_XTYPEOF_TPL(zelda::declval<X>()(ZELDA_PP_PARAMS_Z(z, n, zelda::declval<T, >() BOOST_PP_INTERCEPT))) type; \
    };
    BOOST_PP_REPEAT_1(ZELDA_PARAMS_LIMIT, ZELDA_DEFER_ADAPTOR, ~)
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
