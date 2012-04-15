/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    integral_constant.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_TPL_INTEGRAL_CONSTANT_H
#define ZELDA_GUARD_TPL_INTEGRAL_CONSTANT_H

namespace zelda { namespace tpl {

template< typename T, T N >
struct integral_constant
{
    typedef integral_constant<T, N> type;
    static const T value = N;
    operator T()
    {
        return N;
    }
};

template<bool C>
struct bool_ 
: integral_constant<bool, C> {};

template<int C>
struct int_ 
: integral_constant<int, C> {};

}}

#endif
