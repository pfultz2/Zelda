/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    max_element.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_MAX_ELEMENT_H
#define ZELDA_GUARD_ALGORITHM_MAX_ELEMENT_H

#include <zelda/algorithm/fold.h>


namespace zelda { 

namespace detail {

ZELDA_FUNCTION_CLASS((max_element_fold)(auto x, auto y)(x > y ? x : y))

}

ZELDA_FUNCTION_PIPE_OBJECT((max_element)(auto r)
        if(is_range_or_sequence<r>)(fold(r, detail::max_element_fold()))
    )

}

#endif
