/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    reveal.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_REVEAL_H
#define ZELDA_GUARD_FUNCTION_REVEAL_H

#include <zelda/function/conditional.h>
#include <zelda/function/detail/nullary_tr1_result_of.h>

namespace zelda { 

namespace detail {

template<class F>
struct reveal_adaptor_base : function_adaptor_base<F>
{

    reveal_adaptor_base()
    {}

    template<class X>
    reveal_adaptor_base(X x) : function_adaptor_base<F>(x)
    {}

    struct fail
    {};

    template<class>
    struct result
    {
        typedef fail type;
    };

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
    template<class... T>
    fail operator()(T &&... x) const
    {
        this->get_function()(zelda::forward<T>(x)...);
        return fail();
    }
#else
#define ZELDA_REVEAL_ADAPTOR_BASE(z, n, data) \
    template<ZELDA_PP_PARAMS_Z(z, n, class T)> \
    fail operator()(ZELDA_PP_PARAMS_Z(z, n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT, x)) const \
    { \
        this->get_function()(ZELDA_PP_PARAMS_Z(z, n, zelda::forward<T, > BOOST_PP_INTERCEPT, (x))); \
        return fail(); \
    }
BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_PARAMS_LIMIT, ZELDA_REVEAL_ADAPTOR_BASE, ~)
#endif
};

}

template<class F>
struct reveal_adaptor
: zelda::conditional_adaptor<F, detail::reveal_adaptor_base<F> >
{
    reveal_adaptor()
    {}

    template<class X>
    reveal_adaptor(X x) : zelda::conditional_adaptor<F, detail::reveal_adaptor_base<F> >(x, x)
    {}
};

template<class F>
reveal_adaptor<F> reveal(F f)
{
    return reveal_adaptor<F>(f);
}

}

ZELDA_NULLARY_TR1_RESULT_OF_N(1, zelda::reveal_adaptor)

#if 0
#include <zelda/test.h>

ZELDA_TEST_CASE(reveal_test)
{
}

#endif

#endif
