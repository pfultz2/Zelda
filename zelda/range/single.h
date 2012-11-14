/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    single.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_RANGE_SINGLE_H
#define ZELDA_GUARD_RANGE_SINGLE_H

#include <boost/utility/addressof.hpp>
#include <zelda/range/iterator_range.h>

namespace zelda { 

template <class T>
iterator_range<const T*> single(const T& x)
{
    return make_iterator_range(boost::addressof(x), boost::addressof(x) + 1);
}

template <class T>
iterator_range<T*> single(T& x)
{
    return make_iterator_range(boost::addressof(x), boost::addressof(x) + 1);
}

}

#endif
