/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    inner_product.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_INNER_PRODUCT_H
#define ZELDA_GUARD_ALGORITHM_INNER_PRODUCT_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/empty.hpp>

#include <numeric>

namespace zelda { 

ZELDA_FUNCTION_PIPE_OBJECT((inner_product)(auto r1, auto r2, x)
    if (is_range<r1>, is_range<r2>)
    (
        std::inner_product(boost::begin(r1), boost::end(r1), boost::begin(r2), x)
    )

)
}

#endif
