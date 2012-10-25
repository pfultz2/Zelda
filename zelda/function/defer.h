/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    defer.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_DEFER_H
#define ZELDA_GUARD_FUNCTION_DEFER_H

#include <zelda/function/variadic.h>

namespace zelda {

namespace detail {

template<class F>
struct defer_adaptor_base : function_adaptor_base<F>
{
    defer_adaptor_base() 
    {}

    template<class X>
    defer_adaptor_base(X x) : function_adaptor_base<F>(x) 
    {}

    template<class X>
    struct result;

    template<class X, class T>
    struct result<X(T)> 
    {
        typedef ZELDA_XTYPEOF_TPL(invoke(zelda::declval<F>(), zelda::declval<T>())) type;
    }; 

#ifndef ZELDA_NO_RVALUE_REFS
    template<class T>
    typename result<F(T&&)>::type operator()(T && x) const
    {
        return invoke(this->get_function(), x);
    }
#else
    template<class T>
    typename result<F(const T&)>::type operator()(const T & x) const
    {
        return invoke(this->get_function(), x);
    }

#endif
};

}


template<class F>
struct defer_adaptor : variadic_adaptor<detail::defer_adaptor_base<F> >
{
    defer_adaptor()
    {};

    template<class X>
    defer_adaptor(X x) : variadic_adaptor<detail::defer_adaptor_base<F> >(x)
    {}
};


template<class F>
defer_adaptor<F> defer(F f)
{
    return defer_adaptor<F>(f);
}

}

#endif
