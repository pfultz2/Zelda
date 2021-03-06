/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    perfect_facade.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_MSVC_PERFECT_FACADE_H
#define ZELDA_GUARD_MSVC_PERFECT_FACADE_H

#include <zelda/pp.h>
#include <zelda/config.h>
#include <zelda/function/result_of.h>
#include <zelda/forward.h>

#define ZELDA_DETAIL_PERFECT_FACADE(z, n, F, function) \
template<ZELDA_PP_PARAMS_Z(z, n, class T)> \
typename zelda::result_of<F(ZELDA_PP_PARAMS_Z(z, n, T, && BOOST_PP_INTERCEPT))>::type \
operator()(ZELDA_PP_PARAMS_Z(z, n, T, && x)) const \
{ return function( ZELDA_PP_PARAMS_Z(z, n, zelda::forward<T, > BOOST_PP_INTERCEPT, (x)) ); }

#define ZELDA_DETAIL_PERFECT_FACADE_OP(z, n, data) ZELDA_PP_MSVC_INVOKE(ZELDA_DETAIL_PERFECT_FACADE, (z, n, ZELDA_PP_REM data))
#define ZELDA_PERFECT_FACADE(type, f) BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_PARAMS_LIMIT, ZELDA_DETAIL_PERFECT_FACADE_OP, (type, f))

//Template parameters
#define ZELDA_DETAIL_PERFECT_FACADE_TPL(z, n, F, function, tpl) \
template<tpl T_ ## tpl, ZELDA_PP_PARAMS_Z(z, n, class T)> \
typename zelda::result_of<F(ZELDA_PP_PARAMS_Z(z, n, T, && BOOST_PP_INTERCEPT))>::type \
operator()(ZELDA_PP_PARAMS_Z(z, n, T, && x)) const \
{ return function( ZELDA_PP_PARAMS_Z(z, n, zelda::forward<T, > BOOST_PP_INTERCEPT, (x)) ); }

#define ZELDA_DETAIL_PERFECT_FACADE_OP_TPL(z, n, data) ZELDA_PP_MSVC_INVOKE(ZELDA_DETAIL_PERFECT_FACADE_TPL, (z, n, ZELDA_PP_REM data))
#define ZELDA_PERFECT_FACADE_TPL(type, f, tpl) BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_PARAMS_LIMIT, ZELDA_DETAIL_PERFECT_FACADE_OP_TPL, (type, f, tpl))


#endif
