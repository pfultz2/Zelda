/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    for_each.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_FOR_EACH_H
#define ZELDA_GUARD_ALGORITHM_FOR_EACH_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <algorithm>

namespace zelda { 

ZELDA_FUNCTION_PIPE_OBJECT((for_each)(r, f)
        if(is_range<r>)(std::for_each(boost::begin(r), boost::end(r), f))
        else if(is_sequence<r>)(boost::fusion::for_each(r, f))
    )


}

#endif
