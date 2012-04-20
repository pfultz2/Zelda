/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    not.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_TPL_NOT_H
#define ZELDA_GUARD_TPL_NOT_H

#include <zelda/tpl/integral_constant.h>

namespace zelda { namespace tpl {

template<class B>
struct not_
: bool_<not B::type::value> {};


}}

#endif
