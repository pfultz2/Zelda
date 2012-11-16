/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    unfold.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_RANGE_UNFOLD_H
#define ZELDA_GUARD_RANGE_UNFOLD_H

#include <zelda/iterator/unfold_iterator.h>
#include <zelda/range/iterator_range.h>
#include <zelda/function/builder.h>

namespace zelda { 

template<class State, class Next, class Operate>
iterator_range<unfold_iterator<State, Next, Operate> >
unfold(const State& init, Operate op, Next next)
{
    return iterator_range<unfold_iterator<State, Operate, Next> >
    (
        unfold_iterator<State, Next, Operate>(init, op, next),
        unfold_iterator<State, Next, Operate>()
    );
}

}

#endif
