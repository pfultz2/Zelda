/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    static.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_STATIC_H
#define ZELDA_GUARD_FUNCTION_STATIC_H

#include <zelda/function/adaptor.h>
#include <zelda/function/result_of.h>

#if !defined(ZELDA_NO_VARIADIC_TEMPLATES) && !defined(ZELDA_NO_RVALUE_REFS)
#include <zelda/function/detail/c11/perfect_facade.h>
#elif !defined(ZELDA_NO_RVALUE_REFS)
#include <zelda/function/detail/msvc/perfect_facade.h>
#else
#include <zelda/function/detail/c03/perfect_facade.h>
#endif

namespace zelda { 

// TODO: Add support for forwarding nullary functions as well
template<class F>
struct static_
{

    typedef F function;
    template<class S>
    struct result
    : zelda::result_of<S>::template apply_function<F>
    {};

    ZELDA_PERFECT_FACADE(F, F())
};

}

#ifdef ZELDA_TEST
#include <zelda/test.h>
#include <zelda/function/detail/test.h>

zelda::static_<binary_class> binary_static = {};

zelda::static_<void_class> void_static = {};

zelda::static_<mono_class> mono_static = {};

ZELDA_TEST_CASE(static_test)
{
    void_static(1);
    ZELDA_TEST_EQUAL(3, binary_static(1, 2));
    ZELDA_TEST_EQUAL(3, mono_static(2));
}
#endif

#endif
