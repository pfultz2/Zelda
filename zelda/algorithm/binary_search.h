/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    binary_search.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_BINARY_SEARCH_H
#define ZELDA_GUARD_ALGORITHM_BINARY_SEARCH_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <zelda/algorithm/any_of.h>
#include <algorithm>

#include <boost/bind.hpp>
#include <functional>

namespace zelda { 

namespace detail {

struct binary_search_f
{
    template<class Range, class T>
    typename boost::range_reference<Range>::type
    operator()(ZELDA_FORWARD_REF(Range) r, const T& x) const
    {
        return std::binary_search(r, x);
    }

    template<class Range, class T, class Compare>
    typename boost::range_reference<Range>::type
    operator()(ZELDA_FORWARD_REF(Range) r, const T& x, Compare c) const
    {
        return std::binary_search(r, x, c);
    }
};

}

zelda::static_<zelda::pipable_adaptor<detail::binary_search_f> > binary_search = {};

}

#endif
