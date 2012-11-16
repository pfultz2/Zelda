/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    cycle.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_RANGE_CYCLE_H
#define ZELDA_GUARD_RANGE_CYCLE_H

#include <zelda/iterator/cycle_iterator.h>
#include <zelda/range/iterator_range.h>


namespace zelda { 

template<class Range>
iterator_range<cycle_iterator<typename boost::range_iterator<Range>::type> >
cycle(Range& r, unsigned int n=std::numeric_limits<unsigned int>::max())
{
    typedef cycle_iterator<typename boost::range_iterator<Range>::type> iterator;
    return iterator_range<iterator>
    (
        iterator(boost::begin(r), boost::end(r)), 
        iterator(boost::end(r), n, boost::begin(r), boost::end(r))
    );
}

}

#endif
