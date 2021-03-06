/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    implicit.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_IMPLICIT_H
#define ZELDA_GUARD_FUNCTION_IMPLICIT_H

// @begin
// implicit
// ========
// 
// Description
// -----------
// 
// The `implicit` adaptor is a static function adaptor that uses the type that
// return value can be converted to in order to determine the type of the
// template parameter. Since it is static function adaptor, the function must be
// default constructible.
// 
// Synopsis
// --------
// 
//     template<template <class> class F>
//     class implicit<F>;
// 
// Example
// -------
// 
//     template<class T>
//     struct auto_caster
//     {
//         template<class X>
//         struct result
//         {
//             typedef T type;
//         };
//         template<class U>
//         T operator()(U x)
//         {
//             return T(x);
//         }
//     };
// 
//     implicit<auto_caster> auto_cast = {};
// 
//     struct auto_caster_foo
//     {
//         int i;
//         explicit auto_caster_foo(int i) : i(i) {}
// 
//     };
// 
//     float f = 1.5;
//     int i = auto_cast(f);
//     auto_caster_foo x = auto_cast(1);
//     assert(1 == i);
//     assert(1 == x.i);
// 
// @end

#include <zelda/function/variadic.h>
#include <zelda/function/invoke.h>
#include <zelda/function/static.h>

namespace zelda { namespace detail {

template<template <class> class F, class Sequence>
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

template<template <class> class F>
struct implicit_base
{
    template<class>
    struct result;

    template<class X, class T>
    struct result<X(T)>
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

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
template<template <class> class F>
struct implicit
{
    typedef variadic_adaptor<detail::implicit_base<F> > function;
    template<class S>
    struct result
    : zelda::result_of<S, function>
    {};

    ZELDA_PERFECT_FACADE(function, function())

};
#else

template<template <class> class F, class Enable=void>
struct implicit;

template<template <class> class F>
struct implicit<F, ZELDA_CLASS_REQUIRES(exclude is_callable<variadic_adaptor<detail::implicit_base<F> >()>)>
{
    typedef variadic_adaptor<detail::implicit_base<F> > function;
    template<class S>
    struct result
    : zelda::result_of<S, function>
    {};

    ZELDA_PERFECT_FACADE(function, function())

};

template<template <class> class F>
struct implicit<F, ZELDA_CLASS_REQUIRES(is_callable<variadic_adaptor<detail::implicit_base<F> >()>)>
{
    typedef variadic_adaptor<detail::implicit_base<F> > function;
    template<class S>
    struct result
    : zelda::result_of<S, function>
    {};

    typename zelda::result_of<function()>::type operator()() const
    {
        return function()();
    }

    ZELDA_PERFECT_FACADE(function, function())

};
#endif



}


#ifdef ZELDA_TEST
#include <zelda/test.h>


template<class T>
struct auto_caster
{
    template<class X>
    struct result
    {
        typedef T type;
    };
    template<class U>
    T operator()(U x)
    {
        return T(x);
    }
};

struct auto_caster_foo
{
    int i;
    explicit auto_caster_foo(int i) : i(i) {}

};

zelda::implicit<auto_caster> auto_cast = {};

ZELDA_TEST_CASE(implicit_test)
{
    float f = 1.5;
    int i = auto_cast(f);
    // auto_caster_foo x = 1;
    auto_caster_foo x = auto_cast(1);
    ZELDA_TEST_EQUAL(1, i);
    ZELDA_TEST_EQUAL(1, x.i);

}
#endif

#endif
