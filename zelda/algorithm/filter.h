/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    filter.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_FILTER_H
#define ZELDA_GUARD_ALGORITHM_FILTER_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/iterator/filter_iterator.hpp>
#include <zelda/range/iterator_range.h>

#include <algorithm>

namespace zelda { 

ZELDA_FUNCTION_PIPE_OBJECT((filter)(auto r, f)
    if (is_range<r>)
    (
        zelda::make_iterator_range
        (
            boost::make_filter_iterator(f, boost::begin(r)),
            boost::make_filter_iterator(f, boost::end(r))
        )
    )

    )

}

#endif
