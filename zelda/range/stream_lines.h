/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    stream_lines.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_RANGE_STREAM_LINES_H
#define ZELDA_GUARD_RANGE_STREAM_LINES_H

#include <zelda/range/generate.h>
#include <boost/optional.hpp>
#include <string>

namespace zelda { namespace detail {

struct stream_lines_gen
{
    template<class Stream>
    boost::optional<std::string> operator()(Stream & s) const
    {
        std::string result;
        if (std::getline(s, result)) return result;
        else return boost::optional<std::string>();
    }

    template<class Stream>
    boost::optional<std::string> operator()(Stream & s, char delim) const
    {
        std::string result;
        if (std::getline(s, result, delim)) return result;
        else return boost::optional<std::string>();
    }
};

}
// TODO: Allow custom delimiter
ZELDA_FUNCTION_OBJECT((stream_lines)(s)
    (
        generate(boost::bind(detail::stream_lines_gen(), s))
    ))

}

#endif
