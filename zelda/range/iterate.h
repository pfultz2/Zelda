/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    iterate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_RANGE_ITERATE_H
#define ZELDA_GUARD_RANGE_ITERATE_H

#include <zelda/range/unfold.h>
#include <boost/optional.hpp>

namespace zelda { 

namespace detail {

struct just
{
    template<class T>
    boost::optional<T> operator()(T x) const
    {
        return x;
    }
};

}


ZELDA_FUNCTION_OBJECT((iterate)(init, f)(unfold(init, detail::just(), f)))

}

#endif
