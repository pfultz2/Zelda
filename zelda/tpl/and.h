/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    and.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_TPL_AND_H
#define ZELDA_GUARD_TPL_AND_H

#include <zelda/tpl/fold.h>
#include <zelda/tpl/integral_constant.h>

namespace zelda { namespace tpl {

namespace details{
struct and_op
{
    template<class X, class Y>
    struct apply
    : bool_<X::type::value and Y::type::value> {};
};
}

template<class... Ts>
struct and_
: fold<and_<Ts...>, bool_<true>, details::and_op > {};

}}

#endif
