/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    directory_range.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_RANGE_DIRECTORY_RANGE_H
#define ZELDA_GUARD_RANGE_DIRECTORY_RANGE_H

#include <boost/filesystem/operations.hpp>
#include <zelda/range/iterator_range.h>

namespace zelda { 

inline iterator_range<boost::filesystem::directory_iterator>
    directory_range(const boost::filesystem::path& p)
{
    return make_iterator_range(boost::filesystem::directory_iterator(p),
                               boost::filesystem::directory_iterator());
}

inline iterator_range<boost::filesystem::recursive_directory_iterator>
    recursive_directory_range(const boost::filesystem::path& p)
{
    return make_iterator_range(boost::filesystem::recursive_directory_iterator(p),
                               boost::filesystem::recursive_directory_iterator());
}

}

#endif
