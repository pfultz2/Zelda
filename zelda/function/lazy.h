/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    lazy.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_LAZY_H
#define ZELDA_GUARD_FUNCTION_LAZY_H

#include <zelda/function/adaptor.h>
#include <boost/phoenix/function/function.hpp>
#include <boost/phoenix/core/is_actor.hpp>

namespace zelda { 

template<class F>
struct lazy_adaptor : boost::phoenix::function<F>
{
    lazy_adaptor() {}

    template<class X>
    lazy_adaptor(X x) : boost::phoenix::function<F>(x)
    {}
};

//lazy
//TODO: Use boost::phoenix::detail::expression::function_eval instead
template<class F>
lazy_adaptor<F> lazy(F f)
{
    return boost::phoenix::function<F>(f);
}

}

#endif
