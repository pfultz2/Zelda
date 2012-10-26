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

template<class F>
struct fuse_adaptor : function_adaptor_base<F>
{
    fuse_adaptor() {};

    template<class X>
    fuse_adaptor(X x) : function_adaptor_base<F>(x)
    {};

    template<class X>
    struct result;

    template<class X, class T>
    struct result<X(T)>
    : invoke_result<F, const typename boost::decay<T>::type&> 
    {}; 

    template<class T>
    typename result<F(const T&)>::type operator()(const T & x) const
    {
        return invoke(this->get_function(), x);
    }
};

template<class F>
fuse_adaptor<F> fuse(F f)
{
    return fuse_adaptor<F>(f);
}

}

#endif
