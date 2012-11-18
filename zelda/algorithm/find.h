/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    find.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_FIND_H
#define ZELDA_GUARD_ALGORITHM_FIND_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <algorithm>
#include <string>

namespace zelda { 

namespace detail {

template<class Range, class T>
typename boost::range_iterator<Range>::type
find(const Range& r, const T& x)
{
    return std::find(boost::begin(r), boost::end(r), x);
}

template<class Range, class T>
typename boost::range_iterator<Range>::type
adl_find(const Range& r, const T& x)
{
    return find(r, x);
}

}

ZELDA_FUNCTION_PIPE_OBJECT((find)(r, x)
        if (is_sub_range<r, x>)(std::search(boost::begin(r), boost::end(r), boost::begin(x), boost::end(x)))
        else if (is_range<r>)(detail::adl_find(r, x))
    )   


}

#endif
