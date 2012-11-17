/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    none_of.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_NONE_OF_H
#define ZELDA_GUARD_ALGORITHM_NONE_OF_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <zelda/algorithm/any_of.h>


namespace zelda { 

ZELDA_FUNCTION_PIPE_OBJECT((none_of)(r, f)
        if (is_range_or_sequence<r>)(not any_of(r, f))
    )

}

#endif
