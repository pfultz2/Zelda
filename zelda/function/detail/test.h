/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    test.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_DETAIL_TEST_H
#define ZELDA_GUARD_DETAIL_TEST_H

#include <zelda/function/defer.h>
#include <zelda/forward.h>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>

struct binary_class_d
{
    template<class T, class U>
    T operator()(T x, U y) const
    {
        return x+y;
    }

};

typedef zelda::defer_adaptor<binary_class_d> binary_class;

struct mutable_class
{
    template<class F>
    struct result;

    template<class F, class T, class U>
    struct result<F(T&, U)>
    {
        typedef T type;
    };

    template<class T, class U>
    T operator()(T & x, U y) const
    {
        return x+=y;
    }

};

struct unary_class
{
    template<class F>
    struct result;

    template<class F, class T>
    struct result<F(T)>
    {
        typedef typename zelda::add_forward_reference<T>::type type;
    };

    template<class T>
    ZELDA_FORWARD_REF(T) operator()(ZELDA_FORWARD_REF(T) x) const
    {
        return zelda::forward<T>(x);
    }

};

struct void_class
{
    template<class F>
    struct result;

    template<class F, class T>
    struct result<F(T)>
    {
        typedef void type;
    };

    template<class T>
    void operator()(T) const
    {
    }
};

struct mono_class
{
    template<class F>
    struct result;

    template<class F, class T>
    struct result<F(T)>
    {
        typedef int type;
    };

    int operator()(int x) const
    {
        return x+1;
    }
};

struct tuple_class
{
    template<class F>
    struct result;

    template<class F, class T>
    struct result<F(T)>
    {
        typedef int type;
    };

    template<class T>
    int operator()(T t) const
    {
        return boost::fusion::at_c<0>(t) + 1;
    }
};

#endif
