/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    returns.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ZELDA_RETURNS_H
#define ZELDA_GUARD_ZELDA_RETURNS_H


#define ZELDA_RETURNS(...) -> ZELDA_XTYPEOF(__VA_ARGS__) { return __VA_ARGS__; }

#define ZELDA_ERROR_RETURN_REQUIRES_NEEDS_AN_EXPRESSION(...) ZELDA_XTYPEOF(__VA_ARGS__)>::type { return __VA_ARGS__; }
#define ZELDA_RETURN_REQUIRES(...) -> typename zelda::tpl::if_<ZELDA_DETAIL_REQUIRES_CLAUSE(__VA_ARGS__), ZELDA_ERROR_RETURN_REQUIRES_NEEDS_AN_EXPRESSION

#endif
