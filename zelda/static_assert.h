/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    static_assert.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/


#ifndef ZELDA_GUARD_STATIC_ASSERT_H
#define ZELDA_GUARD_STATIC_ASSERT_H

#ifdef ZELDA_NO_STATIC_ASSERT
#include <boost/static_assert.hpp>
#define ZELDA_DETAIL_STATIC_ASSERT_SEQ(seq) \
BOOST_STATIC_ASSERT((BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_POP_BACK(seq))))

#define static_assert(...) ZELDA_DETAIL_STATIC_ASSERT_SEQ(ZELDA_PP_ARGS_TO_SEQ(__VA_ARGS__))
#endif

#endif