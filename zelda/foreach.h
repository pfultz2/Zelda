/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    foreach.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ZELDA_FOREACH_H
#define ZELDA_GUARD_ZELDA_FOREACH_H

#include <boost/foreach.hpp>
#include <zelda/typeof.h>
#include <zelda/pp.h>

#define ZELDA_FOREACH_KEYWORD(x) BOOST_PP_CAT(ZELDA_FOREACH_KEYWORD_, x)
#define ZELDA_FOREACH_KEYWORD_const (const)
#define ZELDA_FOREACH_KEYWORD_volatile (volatile)
#define ZELDA_FOREACH_KEYWORD_auto (())

#define ZELDA_FOREACH_PROCESS_AUTO(x, col) \
DETAIL_ZELDA_FOREACH_PROCESS_AUTO_INVOKE_M(BOOST_PP_WHILE(DETAIL_ZELDA_FOREACH_PROCESS_AUTO_INVOKE_P, DETAIL_ZELDA_FOREACH_PROCESS_AUTO_INVOKE_OP, (, x, col, ZELDA_PP_TRUE())))
#ifndef _MSC_VER
#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_INVOKE_P(d, state) DETAIL_ZELDA_FOREACH_PROCESS_AUTO_P state
#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_INVOKE_OP(d, state) DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP state
#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_INVOKE_M(state) DETAIL_ZELDA_FOREACH_PROCESS_AUTO_M state
#else
#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_INVOKE_P(d, state) ZELDA_PP_MSVC_INVOKE(DETAIL_ZELDA_FOREACH_PROCESS_AUTO_P, state)
#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_INVOKE_OP(d, state) ZELDA_PP_MSVC_INVOKE(DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP, state)
#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_INVOKE_M(state) ZELDA_PP_MSVC_INVOKE(DETAIL_ZELDA_FOREACH_PROCESS_AUTO_M, state)
#endif

#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_M(head, tail, col, eof) head tail
#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_P(head, tail, col, eof) eof
#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP(head, tail, col, eof) DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP_I(head, tail, col, ZELDA_FOREACH_KEYWORD(tail))

#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP_I(head, tail, col, x) \
BOOST_PP_IIF(ZELDA_PP_IS_PAREN(x), DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP_KEYWORD, DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP_UFO)(head, tail, col, x)

#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP_KEYWORD(head, tail, col, x) DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP_KEYWORD_I(head, tail, col, x, BOOST_PP_SEQ_HEAD(x))
#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP_UFO(head, tail, col, x) (head, tail, col, ZELDA_PP_FALSE())

#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP_KEYWORD_I(head, tail, col, x, key) \
BOOST_PP_IIF(ZELDA_PP_IS_PAREN(key), DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP_AUTO, DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP_CV)(head, tail, col, x, key)

#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP_AUTO(head, tail, col, x, key) (head ZELDA_TYPEOF((*boost::begin(col))) ZELDA_PP_EAT key, BOOST_PP_SEQ_TAIL(x), col, ZELDA_PP_FALSE())
#define DETAIL_ZELDA_FOREACH_PROCESS_AUTO_OP_CV(head, tail, col, x, key) (head key, BOOST_PP_SEQ_TAIL(x), col, ZELDA_PP_TRUE())

#define ZELDA_FOREACH(var, col) BOOST_FOREACH(ZELDA_FOREACH_PROCESS_AUTO(var, col), col)

namespace zelda { 
}

#ifdef ZELDA_FOREACH_TEST

ZELDA_FOREACH_PROCESS_AUTO(const auto& x, range)
ZELDA_FOREACH_PROCESS_AUTO(const int& x, range)
ZELDA_FOREACH_PROCESS_AUTO(int x, range)
ZELDA_FOREACH_PROCESS_AUTO(auto const & x, range)

ZELDA_FOREACH(const auto& x, range)

#endif

#endif
