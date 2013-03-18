/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    always.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_ALWAYS_H
#define ZELDA_GUARD_FUNCTION_ALWAYS_H

#include <zelda/function/variadic.h>
#include <zelda/function/static.h>

namespace zelda { namespace detail {

template<class T>
struct always_base
{
    T x;
    
    always_base()
    {}
    
    template<class X>
    always_base(X x) : x(x)
    {}

    template<class>
    struct result;

    template<class X, class A>
    struct result<X(A)>
    {
        typedef T type;
    };

    template<class A>
    T operator()(A) const
    {
        return this->x;
    }
};

}

template<class T>
variadic_adaptor<detail::always_base<T> > always(T x)
{
    return variadic_adaptor<detail::always_base<T> >(x);
}

}

#ifdef ZELDA_TEST
#include <zelda/test.h>

ZELDA_TEST_CASE(always_test)
{
    int ten = 10;
    ZELDA_TEST_EQUAL( zelda::always(ten)(1,2,3,4,5), 10 );
}

#endif

#endif
