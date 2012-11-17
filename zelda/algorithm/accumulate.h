/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    accumulate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_ACCUMULATE_H
#define ZELDA_GUARD_ALGORITHM_ACCUMULATE_H

#include <zelda/algorithm/fold.h>


namespace zelda { 

namespace detail {

ZELDA_FUNCTION_CLASS((accumulate_fold)(x, y)(x + y))

}

ZELDA_FUNCTION_PIPE_OBJECT((accumulate)(r, init)
        if(is_range_or_sequence<r>)(fold(r, init, detail::accumulate_fold()))
    )

}

#endif
