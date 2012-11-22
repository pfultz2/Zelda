/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    concat.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_CONCAT_H
#define ZELDA_GUARD_ALGORITHM_CONCAT_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <zelda/algorithm/mbind.h>
#include <algorithm>

#include <boost/bind.hpp>
#include <functional>

namespace zelda { 

namespace detail {

ZELDA_FUNCTION_CLASS((concat_identity)(x)(x))

}

ZELDA_FUNCTION_PIPE_OBJECT((concat)(r)
    if (is_range<r>)(mbind(r, detail::concat_identity()))

)

}

#endif
