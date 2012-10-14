/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    result_of.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_RESULT_OF_H
#define ZELDA_GUARD_RESULT_OF_H

#include <zelda/is_callable.h>
#include <zelda/requires.h>
#include <boost/utility/result_of.hpp>

namespace zelda {

namespace detail {

template<class F, class Enable = void>
struct result_of_impl
{};


template<class F>
struct result_of_impl<F, ZELDA_CLASS_REQUIRES(is_callable<F>)>
: boost::result_of<F> {};

}

template<class Sig>
struct result_of;

#ifndef ZELDA_NO_VARIADIC_TEMPLATES

template<class F, class... T>
struct result_of<F(T...)>
: detail::result_of_impl<F(T...)>
{
    typedef F function_type;

    template<class X>
    struct apply
    : detail::result_of_impl<X(T...)>
    {};
};

#else

#define ZELDA_RESULT_OF_GENERATE(z, n, data) \
template<class F BOOST_PP_COMMA_IF(n) ZELDA_PP_PARAMS_Z(z, n, class T)> \
struct result_of<F(ZELDA_PP_PARAMS_Z(z, n, T))> \
: detail::result_of_impl<F(ZELDA_PP_PARAMS_Z(z, n, T))> \
{ \
    typedef F function_type; \
\
    template<class X> \
    struct apply \
    : detail::result_of_impl<X(ZELDA_PP_PARAMS_Z(z, n, T))> \
    {}; \
};
BOOST_PP_REPEAT_1(ZELDA_PARAMS_LIMIT, ZELDA_RESULT_OF_GENERATE, ~)
#undef ZELDA_RESULT_OF_GENERATE

#endif

}



#endif