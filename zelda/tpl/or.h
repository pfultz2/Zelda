/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    or.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_TPL_OR_H
#define ZELDA_GUARD_TPL_OR_H

#include <zelda/tpl/fold.h>
#include <zelda/tpl/integral_constant.h>

namespace zelda { namespace tpl {

namespace details{
struct or_op
{
    template<class X, class Y>
    struct apply
    : bool_<X::type::value or Y::type::value> {};
};
}

template<class... Ts>
struct or_
: fold<or_<Ts...>, bool_<true>, details::or_op > {};


}}

#endif
