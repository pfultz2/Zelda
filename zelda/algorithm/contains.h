/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    contains.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_CONTAINS_H
#define ZELDA_GUARD_ALGORITHM_CONTAINS_H

#include <zelda/algorithm/find.h>

namespace zelda { 

ZELDA_FUNCTION_PIPE_OBJECT((contains)(r, x)
        if (is_range<r>)(zelda::find(r, x) != boost::end(r))
    )

}

#endif
