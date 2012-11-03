/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    fuse.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_FUSE_H
#define ZELDA_GUARD_FUNCTION_FUSE_H

#include <zelda/function/adaptor.h>
#include <zelda/function/perfect.h>
#include <zelda/function/variadic.h>
#include <zelda/function/invoke.h>

namespace zelda {

template<class F>
struct fuse_adaptor : function_adaptor_base<F>
{
    fuse_adaptor() {};

    template<class X>
    fuse_adaptor(X x) : function_adaptor_base<F>(x)
    {};

    template<class X, class Enable = void>
    struct result;

    template<class X, class T>
    struct result<X(T), ZELDA_CLASS_REQUIRES(boost::fusion::traits::is_sequence<typename boost::decay<T>::type>)>
    : invoke_result<F, const typename boost::decay<T>::type&> 
    {}; 

    template<class T>
    typename result<F(const T&)>::type operator()(const T & x) const
    {
        return invoke(this->get_function(), x);
    }
};

// Optimizations
template<class F>
struct fuse_adaptor<variadic_adaptor<F> > : perfect_adaptor<F>
{
    fuse_adaptor() {};

    template<class X>
    fuse_adaptor(X x) : perfect_adaptor<F>(x)
    {};
};

template<class F>
struct variadic_adaptor<fuse_adaptor<F> > : perfect_adaptor<F>
{
    variadic_adaptor() {};

    template<class X>
    variadic_adaptor(X x) : perfect_adaptor<F>(x)
    {};
};

template<class F>
fuse_adaptor<F> fuse(F f)
{
    return fuse_adaptor<F>(f);
}

}

#endif
