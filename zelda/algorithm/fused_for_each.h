/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    fused_for_each.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_FUSED_FOR_EACH_H
#define ZELDA_GUARD_ALGORITHM_FUSED_FOR_EACH_H

#include <zelda/algorithm/for_each.h>
#include <zelda/function/fuse.h>

namespace zelda {

ZELDA_FUNCTION_PIPE_OBJECT((fused_for_each)(auto r, f)
    (zelda::for_each(r, zelda::fuse(f)))
)

}

#endif
