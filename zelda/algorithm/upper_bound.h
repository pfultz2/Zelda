/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    upper_bound.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_UPPER_BOUND_H
#define ZELDA_GUARD_ALGORITHM_UPPER_BOUND_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/empty.hpp>

#include <algorithm>

namespace zelda { 

ZELDA_FUNCTION_PIPE_OBJECT((upper_bound)(auto r, x)
    if (is_range<r>)
    (
        std::upper_bound(boost::begin(r), boost::end(r), x)
    )

)

}

#endif
