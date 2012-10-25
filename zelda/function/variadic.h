/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    variadic.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_VARIADIC_H
#define ZELDA_GUARD_FUNCTION_VARIADIC_H

#include <zelda/function/adaptor.h>
#include <zelda/function/perfect.h>
#include <zelda/result_of.h>
#include <zelda/tuple.h>


namespace zelda { 

namespace detail {
// TODO: Add a lightweight vardiac adaptor that takes the parameters by value
template<class F>
struct variadic_adaptor_base : function_adaptor_base<F>
{
    variadic_adaptor_base() {}

    template<class X>
    variadic_adaptor_base(X x) : function_adaptor_base<F>(x)
    {}

    template<class X>
    struct result;

#ifndef ZELDA_NO_VARIADIC_TEMPLATES

    template<class X, class... T>
    struct result<X(T...)>
    : zelda::result_of<F(zelda::tuple<T&&...>)> 
    {};

    template<class... T>
    typename zelda::result_of<F(zelda::tuple<T&&...>)>::type
    operator()(T && ... x) const
    {
        return this->get_function()(zelda::tuple<T&&...>(zelda::forward<T>(x)...));
    }
#else

// TODO: Add support for nullary functions
#define ZELDA_FUNCTION_VARIADIC_ADAPTOR(z, n, data) \
    template<class X, ZELDA_PP_PARAMS_Z(z, n, class T)> \
    struct result<X(ZELDA_PP_PARAMS_Z(z, n, T))> \
    : zelda::result_of<F(zelda::tuple<ZELDA_PP_PARAMS_Z(z, n, typename add_tuple_forward_reference<T, >::type BOOST_PP_INTERCEPT)>)> \
    {}; \
    template<ZELDA_PP_PARAMS_Z(z, n, class T)> \
    typename zelda::result_of<F(zelda::tuple<ZELDA_PP_PARAMS_Z(z, n, typename add_tuple_forward_reference<T, >::type BOOST_PP_INTERCEPT)>) >::type \
    operator()(ZELDA_PP_PARAMS_Z(z, n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT, x)) const \
    { \
        return this->get_function()(zelda::tuple<ZELDA_PP_PARAMS_Z(z, n, typename add_tuple_forward_reference<T, >::type BOOST_PP_INTERCEPT)> \
            ( \
                ZELDA_PP_PARAMS_Z(z, n, zelda::forward<T, > BOOST_PP_INTERCEPT, (x)) \
            )); \
    }
BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_PARAMS_LIMIT, ZELDA_FUNCTION_VARIADIC_ADAPTOR, ~)

#endif
};
}

template<class F>
struct variadic_adaptor : perfect_adaptor<detail::variadic_adaptor_base<F> >
{
    variadic_adaptor() {}

    template<class X>
    variadic_adaptor(X x) : perfect_adaptor<detail::variadic_adaptor_base<F> >(x)
    {}
};

template<class F>
variadic_adaptor<F> variadic(F f)
{
    return variadic_adaptor<F>(f);
}




}

#endif
