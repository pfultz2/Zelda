/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    assert.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ZELDA_ASSERT_H
#define ZELDA_GUARD_ZELDA_ASSERT_H


#include <iostream>
#include <boost/current_function.hpp>
#include <zelda/pp.h>
#include <zelda/static_assert.h>
#include <zelda/typeof.h>


#define ZELDA_ASSERT(...) BOOST_PP_CAT(ZELDA_DETAIL_ASSERT_, ZELDA_PP_NARGS(__VA_ARGS__))(__VA_ARGS__)

#if defined(NDEBUG) || defined(ZELDA_DISABLE_ASSERTS)

#define ZELDA_DETAIL_ASSERT_1(cond) ((void)0)
#define ZELDA_DETAIL_ASSERT_2(cond, msg) ((void)0)
#define ZELDA_ASSERT_EXPR(cond, expr) expr

#else

#define ZELDA_DETAIL_ASSERT_1(cond) ((cond) ? ((void)0) : ::zelda::assertion::failed(zelda::assertion::void_(), #cond, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__))
#define ZELDA_DETAIL_ASSERT_2(cond, msg) ((cond) ? ((void)0) : ::zelda::assertion::failed_msg(zelda::assertion::void_(), #cond, msg, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__))
#define ZELDA_ASSERT_EXPR(cond, expr) ((cond) ? (expr) : ::zelda::assertion::failed((ZELDA_AVOID(expr)), #cond, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__))

#endif

namespace zelda { namespace assertion {

typedef zelda::typeof_detail::void_ void_;

template<class T>
struct result
{
    typedef T& type;
};

template<>
struct result<void_>
{
    typedef void type;
};

template<class T>
typename result<T>::type failed_msg(const T&, char const * cond, char const * msg, char const * function, char const * file, long line)
{
    std::cerr
      << "***** Internal Program Error - assertion (" << cond << ") failed in "
      << function << ":\n"
      << file << '(' << line << "): " << msg << std::endl;
    std::abort();
}

template<class T>
typename result<T>::type failed(const T&, char const * cond, char const * function, char const * file, long line)
{
    std::cerr
      << "***** Internal Program Error - assertion (" << cond << ") failed in "
      << function << ":\n"
      << file << '(' << line << ")" << std::endl;
    std::abort();
}

#ifdef ZELDA_TEST
namespace test {

int by_value();
int& by_ref();
const int& by_const_ref();
void by_void();

void check()
{
    ZELDA_ASSERT(true);
    ZELDA_ASSERT(true, "This should work");
    static_assert(not boost::is_reference<ZELDA_XTYPEOF(ZELDA_ASSERT_EXPR(true, by_value()))>::value, "Failed");
    static_assert(not zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(ZELDA_ASSERT_EXPR(true, by_value()))>::value, "Failed");

    static_assert(boost::is_reference<ZELDA_XTYPEOF(ZELDA_ASSERT_EXPR(true, by_ref()))>::value, "Failed");
    static_assert(not zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(ZELDA_ASSERT_EXPR(true, by_ref()))>::value, "Failed");

    static_assert(boost::is_reference<ZELDA_XTYPEOF(ZELDA_ASSERT_EXPR(true, by_const_ref()))>::value, "Failed");
    static_assert(zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(ZELDA_ASSERT_EXPR(true, by_const_ref()))>::value, "Failed");

    static_assert(boost::is_same<ZELDA_XTYPEOF(ZELDA_ASSERT_EXPR(true, by_void())), void>::value, "Failed");
}

}


#endif

}}

#endif
