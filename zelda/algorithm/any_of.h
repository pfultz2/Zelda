/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    any_of.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_ANY_OF_H
#define ZELDA_GUARD_ALGORITHM_ANY_OF_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/fusion/algorithm/query/any.hpp>
#include <algorithm>


namespace zelda { 

ZELDA_FUNCTION_PIPE_OBJECT((any_of)(r, f)
        if (is_sequence<r>)(boost::fusion::any(r, f))
        else if (is_range<r>)(std::find_if(boost::begin(r), boost::end(r), f) != boost::end(r))

    )

}

#endif
