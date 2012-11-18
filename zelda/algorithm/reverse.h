/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    reverse.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_REVERSE_H
#define ZELDA_GUARD_ALGORITHM_REVERSE_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/iterator/reverse_iterator.hpp>
#include <zelda/range/iterator_range.h>

#include <boost/fusion/algorithm/transformation/reverse.hpp>

namespace zelda { 

ZELDA_FUNCTION_PIPE_OBJECT((reverse)(r)
    if (has_range_traversal<r, boost::bidirectional_traversal_tag>)
    (
        make_iterator_range
        (
            boost::make_reverse_iterator(boost::end(r)),
            boost::make_reverse_iterator(boost::begin(r)) 
        )
    )
    else if (is_sequence<r>)
    (
        boost::fusion::reverse(r)
    )

    )

}

#endif
