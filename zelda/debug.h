/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    debug.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ZELDA_DEBUG_H
#define ZELDA_GUARD_ZELDA_DEBUG_H

#include <zelda/traits.h>
#include <zelda/requires.h>
#include <boost/lexical_cast.hpp>
#include <zelda/foreach.h>

namespace zelda { 

template<class T>
ZELDA_FUNCTION_REQUIRES(exclude is_range<T>, exclude is_pair<T>)
(std::string) debug_string(const T& x)
{
    return boost::lexical_cast<std::string>(x);
}

template<class Range>
ZELDA_FUNCTION_REQUIRES(is_range<Range>, exclude is_string<Range>)
(std::string) debug_string(const Range& r);

template<class Pair>
ZELDA_FUNCTION_REQUIRES(is_pair<Pair>)
(std::string) debug_string(const Pair& p)
{
    return debug_string(p.first) + "=" + debug_string(p.second);
}

template<class Range>
ZELDA_FUNCTION_REQUIRES(is_range<Range>, exclude is_string<Range>)
(std::string) debug_string(const Range& r)
{
    std::string out;
    std::string delim = "[";
    ZELDA_FOREACH(const auto& x, r)
    {
        out += delim + debug_string(x);
    }
    out += "]";
    return out;
}

}

#endif
