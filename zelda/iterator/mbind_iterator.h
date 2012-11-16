/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    mbind_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ITERATOR_MBIND_ITERATOR_H
#define ZELDA_GUARD_ITERATOR_MBIND_ITERATOR_H

#include <zelda/function/result_of.h>
#include <zelda/typeof.h>
#include <boost/range.hpp>

namespace zelda { 

template<class OuterIterator, class Selector, class SelectorRange = typename zelda::result_of<Selector(typename boost::iterator_reference<OuterIterator>::type)>::type>
struct mbind_iterator
: boost::iterator_facade
<
    mbind_iterator<OuterIterator, Selector, SelectorRange>,
    typename boost::range_value<SelectorRange >::type,
    boost::forward_traversal_tag,
    typename boost::range_reference<SelectorRange >::type
>
{
    typedef typename boost::range_iterator<SelectorRange >::type InnerIteraror;

    Selector selector;
    OuterIterator iterator;
    InnerIteraror inner_first;
    InnerIteraror inner_last;
    OuterIterator last;

    mbind_iterator(Selector selector, OuterIterator iterator, OuterIterator last) : selector(selector), iterator(iterator), last(last)
    {
        this->select();
    }

    void select()
    {
        for(;iterator!=last;iterator++)
        {
            if (inner_first==inner_last)
            {
                ZELDA_XAUTO(r, selector(*iterator));
                inner_first = boost::begin(r);
                inner_last = boost::end(r);
            }
            else inner_first++;
            for(;inner_first!=inner_last;inner_first++)
                return;
        }
    }

    void increment()
    {
        this->select();
    }

    bool equal(const mbind_iterator& other) const
    {
        return this->iterator == other.iterator;
    }

    typename boost::range_reference<SelectorRange >::type dereference() const
    {
        return *inner_first;
    }

};

template<class Iterator, class Selector>
mbind_iterator<Iterator, Selector> make_mbind_iterator(Selector selector, Iterator iterator, Iterator last)
{
    return mbind_iterator<Iterator, Selector>(selector, iterator, last);
}

}

#endif
