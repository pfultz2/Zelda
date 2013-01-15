/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    min_element.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_MIN_ELEMENT_H
#define ZELDA_GUARD_ALGORITHM_MIN_ELEMENT_H

namespace zelda { 

namespace detail {

ZELDA_FUNCTION_CLASS((min_element_fold)(auto x, auto y)(x < y ? x : y))

}

ZELDA_FUNCTION_PIPE_OBJECT((min_element)(auto r)
        if(is_range_or_sequence<r>)(fold(r, detail::min_element_fold()))
    )

}

#endif
