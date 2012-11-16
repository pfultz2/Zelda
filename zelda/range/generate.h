/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    generate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_RANGE_GENERATE_H
#define ZELDA_GUARD_RANGE_GENERATE_H

#include <zelda/range/unfold.h>
#include <boost/bind.hpp>

namespace zelda {

namespace detail {

struct ignore {};

struct identity
{
    template<class T>
    T operator()(T x) const
    {
        return x;
    }
};

}

ZELDA_FUNCTION_OBJECT((generate)(gen)
    (
        unfold(detail::ignore(), boost::bind(gen), detail::identity())
    ))

}

#endif
