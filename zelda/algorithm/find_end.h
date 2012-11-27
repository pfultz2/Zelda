/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    find_end.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_FIND_END_H
#define ZELDA_GUARD_ALGORITHM_FIND_END_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <algorithm>

namespace zelda { 

ZELDA_FUNCTION_PIPE_OBJECT((find_end)(auto r, x)
        if (is_sub_range<r, x>)(std::find_end(boost::begin(r), boost::end(r), boost::begin(x), boost::end(x)))
    )   


}

#endif
