/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    at.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_AT_H
#define ZELDA_GUARD_ALGORITHM_AT_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <zelda/algorithm/empty.h>

#include <algorithm>

#include <zelda/assert.h>

namespace zelda { 

namespace detail {

template<class R, class I>
ZELDA_FUNCTION_REQUIRES(has_range_traversal<R, boost::random_access_traversal_tag>)
(typename boost::range_iterator<R>::type)
range_at(R& r, I n)
{
    typename boost::range_iterator<const R>::type it;
    if (n < 0) it = boost::end(r);
    else it = boost::begin(r);
    return it += n;
}

}

ZELDA_FUNCTION_PIPE_OBJECT((at)(r, n)
        if (has_range_traversal<r, boost::random_access_traversal_tag>)
        (
            ZELDA_ASSERT_EXPR(!zelda::empty(r), detail::range_at(r))
        )

    )


}

#endif
