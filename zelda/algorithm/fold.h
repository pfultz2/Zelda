/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    fold.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_FOLD_H
#define ZELDA_GUARD_ALGORITHM_FOLD_H


#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <numeric>

namespace zelda { 

// TODO: fold without an initial value
ZELDA_FUNCTION_PIPE_OBJECT((fold)(r, init, f)
        if(is_range<r>)(std::accumulate(boost::begin(r), boost::end(r), init, f))
        else if(is_sequence<r>)(boost::fusion::fold(r, init, f))
    )

}

#endif
