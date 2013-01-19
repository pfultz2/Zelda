/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    implicit.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_IMPLICIT_H
#define ZELDA_GUARD_FUNCTION_IMPLICIT_H

#include <zelda/function/variadic.h>
#include <zelda/function/invoke.h>
#include <zelda/function/static.h>

namespace zelda { namespace detail {

template<template <class> F, class Sequence>
struct implicit_invoke
{
    Sequence seq;

    implicit_invoke(Sequence seq) : seq(seq)
    {}

    // TODO: Add a default template parameter in c++11 to check if it is callable
    template<class X>
    operator X() const
    {
        return invoke(F<X>(), seq);
    }
};

template<template <class> F>
struct implicit_base
{
    template<class>
    struct result;

    template<class X, class T>
    struct result
    {
        typedef implicit_invoke<F, typename boost::decay<T>::type> type;
    };

    template<class T>
    implicit_invoke<F, T> operator()(const T& x) const
    {
        return implicit_invoke<F, T>(x);
    }
};

}

template<template <class> F>
struct implicit : static_<variadic_adaptor<detail::implicit_base<F> > >
{};


}

#endif
