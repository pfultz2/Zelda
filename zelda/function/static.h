/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    static.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_STATIC_H
#define ZELDA_GUARD_FUNCTION_STATIC_H

// @begin
// static
// ======
// 
// Description
// -----------
// 
// The `static_` adaptor is a static function adaptor that allows any default-
// constructible function object to be static-initialized.
// 
// Synopsis
// --------
// 
//     template<class F>
//     class static_;
// 
// Example
// -------
// 
//     // In C++03 this class can't be static-initialized, because of the 
//     // default constructor.
//     struct times_function
//     {
//         double factor;
//         times2_function() : factor(2)
//         {}
//         template<class T>
//         T operator()(T x) const
//         {
//             return x*factor;
//         }
//     };
// 
//     static_<times_function> times2 = {};
// 
//     assert(6 == times2(3));
// 
// @end

#include <zelda/function/adaptor.h>
#include <zelda/function/result_of.h>
#include <zelda/function/detail/nullary_tr1_result_of.h>

#if !defined(ZELDA_NO_VARIADIC_TEMPLATES) && !defined(ZELDA_NO_RVALUE_REFS)
#include <zelda/function/detail/c11/perfect_facade.h>
#elif !defined(ZELDA_NO_RVALUE_REFS)
#include <zelda/function/detail/msvc/perfect_facade.h>
#else
#include <zelda/function/detail/c03/perfect_facade.h>
#endif

namespace zelda { 

// TODO: Add support for forwarding nullary functions as well
#ifndef ZELDA_NO_VARIADIC_TEMPLATES
template<class F>
struct static_
{
    typedef F function;
    template<class S>
    struct result
    : zelda::result_of<S, function>
    {};

    ZELDA_PERFECT_FACADE(function, function())
};

#else
template<class F, class Enable = void>
struct static_;

template<class F>
struct static_<F, ZELDA_CLASS_REQUIRES(exclude is_callable<F()>)>
{
    typedef F function;
    template<class S>
    struct result
    : zelda::result_of<S, function>
    {};

    ZELDA_PERFECT_FACADE(function, function())
};

template<class F>
struct static_<F, ZELDA_CLASS_REQUIRES(is_callable<F()>)>
{
    typedef F function;
    template<class S>
    struct result
    : zelda::result_of<S, function>
    {};

    typename zelda::result_of<F()>::type operator()() const
    {
        return function()();
    }

    ZELDA_PERFECT_FACADE(function, function())
};

#endif

}

ZELDA_NULLARY_TR1_RESULT_OF_N(1, zelda::static_)

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
