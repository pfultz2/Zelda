/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    join.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_JOIN_H
#define ZELDA_GUARD_ALGORITHM_JOIN_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/range/join.hpp>
#include <zelda/range/iterator_range.h>

#include <boost/fusion/algorithm/transformation/join.hpp>

namespace zelda { 

ZELDA_FUNCTION_PIPE_OBJECT((join)(auto x, auto y)
    if (is_range<x>, is_range<y>)(boost::range::join(x, y))
    else if (is_sequence<x>, is_sequence<y>)(boost::fusion::join(x, y))

    )


}

#endif
