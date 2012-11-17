/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    all_of.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_ALL_OF_H
#define ZELDA_GUARD_ALGORITHM_ALL_OF_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <zelda/algorithm/any_of.h>
#include <algorithm>

#include <boost/bind.hpp>
#include <functional>

namespace zelda { 

ZELDA_FUNCTION_PIPE_OBJECT((all_of)(r, f)
        if (is_range_or_sequence<r>)
            (
                any_of(r, boost::bind(std::logical_not<bool>(), boost::bind(f, _1)))
            )
    )

}

#endif
