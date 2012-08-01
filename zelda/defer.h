/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    defer.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_DEFER_H
#define ZELDA_GUARD_DEFER_H

#include <zelda/requires.h>
#include <zelda/typeof.h>
#include <boost/type_traits/is_empty.hpp>

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
#define ZELDA_DEFER_RESULT() \
template<class X> \
struct result; \
template<class X, class... T> \
struct result<X(T...)> \
{ \
    typedef ZELDA_XTYPEOF_TPL(zelda::declval<F>()(zelda::declval<T>()...)) type; \
};

#else
#define ZELDA_DEFER_RESULT() \
template<class X> \
struct result; \
BOOST_PP_REPEAT_1(10, ZELDA_DEFER_RESULT_EACH, ~)

#define ZELDA_DEFER_RESULT_EACH(z, n, data) \
template<class X BOOST_PP_COMMA_IF(n)  ZELDA_PP_PARAMS(n, class T)> \
struct result<X(ZELDA_PP_PARAMS(n, T))> \
{ \
    typedef ZELDA_XTYPEOF_TPL(zelda::declval<F>()( ZELDA_PP_PARAMS(n, zelda::declval<T, >() BOOST_PP_INTERCEPT) )) type; \
};

#endif

namespace zelda { 

template<class F, class Enable = void>
struct defer_adaptor : F
{
    defer_adaptor() {};
    template<class T>
    defer_adaptor(T x) : F(x) {};

    ZELDA_DEFER_RESULT()

};

template<class F>
struct defer_adaptor<F, ZELDA_CLASS_REQUIRES(boost::is_empty<F>)> : F
{
    ZELDA_DEFER_RESULT()
};

template<class F>
defer_adaptor<F> defer(F f)
{
    return defer_adaptor<F>(f);
}

}

#endif