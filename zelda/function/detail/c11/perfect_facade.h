/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    perfect_facade.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_C11_PERFECT_FACADE_H
#define ZELDA_GUARD_C11_PERFECT_FACADE_H

#include <zelda/result_of.h>

namespace zelda { namespace detail {

template<class Derived, class F>
struct perfect_facade
{

    template<class... T>
    typename zelda::result_of<F(T&&...)>::type
    operator()(T && ... x) const
    {
        return static_cast<Derived*>(this)->get_function()(zelda::forward<T>(x)...);
    }
};

}}

#endif
