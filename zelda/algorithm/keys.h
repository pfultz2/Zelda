/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    keys.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_KEYS_H
#define ZELDA_GUARD_ALGORITHM_KEYS_H

#include <zelda/algorithm/transform.h>

namespace zelda { namespace detail {

ZELDA_FUNCTION_CLASS((keys_selector)(x)(x.first))

}

ZELDA_FUNCTION_PIPE_OBJECT((keys)(r)
    if (is_range_or_sequence<r>)(zelda::transform(r, detail::keys_selector()))
)



}

#endif
