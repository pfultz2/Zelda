/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    mbind.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_MBIND_H
#define ZELDA_GUARD_ALGORITHM_MBIND_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <zelda/iterator/mbind_iterator.h>
#include <zelda/range/iterator_range.h>
#include <algorithm>

namespace zelda { 

ZELDA_FUNCTION_PIPE_OBJECT((mbind)(r, f)
    if (is_range<r>)
    (
        make_iterator_range
        (
            make_bind_iterator(r, boost::begin(r), boost::end(r)),
            make_bind_iterator(r, boost::end(r), boost::end(r))
        )
    )

)

}

#endif
