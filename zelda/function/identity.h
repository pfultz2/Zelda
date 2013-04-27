/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    identity.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_IDENTITY_H
#define ZELDA_GUARD_FUNCTION_IDENTITY_H

// @begin
// identity
// ========
// 
// Description
// -----------
// 
// The `identity` function is an unary function object that returns whats given to it. 
// 
// Synopsis
// --------
// 
//     struct
//     {
//         template<class T>
//         T&& operator()(T&& x) const
//         {
//             return zelda::forward<T>(x);
//         }
//     } identity;
// 
// @end

#include <zelda/function/static.h>

namespace zelda { namespace detail {

struct identity_base
{
    template<class F>
    struct result;

    template<class F, class T>
    struct result<F(T)>
    : zelda::add_forward_reference<T>
    {};

    template<class T>
    ZELDA_FORWARD_REF(T) operator()(ZELDA_FORWARD_REF(T) x) const
    {
        return zelda::forward<T>(x);
    }
};

}

static_<detail::identity_base> identity = {};

}

#ifdef ZELDA_TEST
#include <zelda/test.h>

ZELDA_TEST_CASE(identity_test)
{
    ZELDA_TEST_EQUAL( zelda::identity(10), 10 );
}

#endif

#endif
