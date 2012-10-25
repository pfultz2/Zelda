/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    fuse.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_FUSE_H
#define ZELDA_GUARD_FUNCTION_FUSE_H

#include <zelda/tuple.h>
#include <zelda/function/adaptor.h>
#include <zelda/function/perfect.h>

namespace zelda {

namespace detail {
template<class F>
struct fuse_adaptor_base : function_adaptor_base<F>
{
    fuse_adaptor_base() 
    {}

    template<class X>
    fuse_adaptor_base(X x) : function_adaptor_base<F>(x) 
    {}

    template<class X>
    struct result;

    template<class X, class T>
    struct result<X(T)>
    : invoke_result<F, typename boost::decay<T>::type > 
    {}; 

#ifndef ZELDA_NO_RVALUE_REFS
    template<class T>
    typename result<F(T)>::type operator()(T && x) const
    {
        return invoke(this->get_function(), x);
    }
#else
    template<class T>
    typename result<F(T)>::type operator()(const T & x) const
    {
        return invoke(this->get_function(), x);
    }

#endif
};
}

template<class F>
struct fuse_adaptor : perfect_adaptor<detail::fuse_adaptor_base<F> >
{
    fuse_adaptor() {};

    template<class X>
    fuse_adaptor(X x) : perfect_adaptor<detail::fuse_adaptor_base<F> >(x)
    {};
};

template<class F>
fuse_adaptor<F> fuse(F f)
{
    return fuse_adaptor<F>(f);
}

}

#endif
