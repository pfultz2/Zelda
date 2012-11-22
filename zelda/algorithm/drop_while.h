/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    drop_while.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_DROP_WHILE_H
#define ZELDA_GUARD_ALGORITHM_DROP_WHILE_H

#include <zelda/algorithm/find_if.h>
#include <zelda/range/iterator_range.h>

namespace zelda { 

ZELDA_FUNCTION_PIPE_OBJECT((drop_while)(r, f)
    if (is_range<r>)(make_iterator_range(find_if(r, f), boost::end(r)))

)

}

#endif
