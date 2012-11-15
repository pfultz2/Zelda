/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    empty_range.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_RANGE_EMPTY_RANGE_H
#define ZELDA_GUARD_RANGE_EMPTY_RANGE_H

#include <zelda/range/iterator_range.h>

namespace zelda {

template<class Value>
struct empty_range
: iterator_range<Value *>
{
    typedef iterator_range<Value *> base;

    empty_range() : base(0,0)
    {}
};

}

#endif
