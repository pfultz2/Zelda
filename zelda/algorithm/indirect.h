/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    indirect.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_INDIRECT_H
#define ZELDA_GUARD_ALGORITHM_INDIRECT_H

#include <zelda/algorithm/transform.h>

namespace zelda { namespace detail {

ZELDA_FUNCTION_CLASS((indirect_selector)(x)(*x))

}

ZELDA_FUNCTION_PIPE_OBJECT((indirect)(r)
    if (is_range_or_sequence<r>)(zelda::transform(r, detail::indirect_selector()))
)


}

#endif
