/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    count.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_COUNT_H
#define ZELDA_GUARD_ALGORITHM_COUNT_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/fusion/algorithm/query/count.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <algorithm>

namespace zelda {

namespace detail {

template<class R>
std::ptrdiff_t size(const R & x)
{
    // TODO: Create a distance that recognizes iterator_traversals
    return std::distance(boost::begin(x), boost::end(x));
}

template<class R>
std::ptrdiff_t adl_size(const R& x)
{
    return size(x);
}

struct count
{
    template<class R>
    ZELDA_FUNCTION_REQUIRES(is_range<R>)
    (std::ptrdiff_t) operator()(const R& r) const
    {
        return adl_size(r);
    }

    template<class R, class T>
    ZELDA_FUNCTION_REQUIRES(is_range<R>)
    (std::ptrdiff_t) operator()(const R& r, const T& x) const
    {
        return std::count(boost::begin(r), boost::end(r), x);
    }

    template<class R>
    ZELDA_FUNCTION_REQUIRES(is_sequence<R>, exclude is_range<R>)
    (std::ptrdiff_t) operator()(const R& r) const
    {
        return boost::fusion::size(r);
    }

    template<class R, class T>
    ZELDA_FUNCTION_REQUIRES(is_sequence<R>, exclude is_range<R>)
    (std::ptrdiff_t) operator()(const R& r, const T& x) const
    {
        return boost::fusion::count(r, x);
    }

};
}

static_<pipable_adaptor<detail::count> > count = {};

}

#endif
