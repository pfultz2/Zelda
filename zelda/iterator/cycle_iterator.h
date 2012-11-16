/*=============================================================================
    Copyright (c) 2005-2007 Shunsuke Sogame
    cycle_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ITERATOR_CYCLE_ITERATOR_H
#define ZELDA_GUARD_ITERATOR_CYCLE_ITERATOR_H

#include <utility>
#include <limits>
#include <boost/assert.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/next_prior.hpp>
#include <boost/type_traits/is_same.hpp>

namespace zelda { 

namespace detail {

template< class Difference >
std::pair<Difference, Difference> positive_rem_div(Difference a, Difference b)
{
    assert(b >= 0);

    Difference const q = a / b;
    Difference const r = a % b;
    if (r < 0)
        return std::make_pair(q - 1, r + b);
    else
        return std::make_pair(q, r);
}

}

template< class ForwardIterator, class Incrementable=unsigned int >
struct cycle_iterator :
    boost::iterator_adaptor<cycle_iterator<ForwardIterator, Incrementable>, ForwardIterator>
{
    typedef typename boost::iterator_adaptor<cycle_iterator<ForwardIterator, Incrementable>, ForwardIterator> super;
    typedef typename super::difference_type difference_type;
    typedef typename super::reference reference;

    Incrementable depth;
    ForwardIterator first, last;

    cycle_iterator()
    { }

    cycle_iterator(
        ForwardIterator first, ForwardIterator last
    ) :
        super(first), depth(0),
        first(first), last(last)
    {
    }

    cycle_iterator(
        ForwardIterator it,
        ForwardIterator first, ForwardIterator last
    ) :
        super(it), depth(std::numeric_limits<Incrementable>::max()),
        first(first), last(last)
    {
    }

    cycle_iterator(
        ForwardIterator it, Incrementable depth,
        ForwardIterator first, ForwardIterator last
    ) :
        super(it), depth(depth),
        first(first), last(last)
    {
    }

    template< class, class > friend struct cycle_iterator;

    template< class F, class I >
    cycle_iterator(cycle_iterator<F, I> const& other,
        typename boost::enable_if_convertible<F, ForwardIterator>::type * = 0,
        typename boost::enable_if_convertible<I, Incrementable>::type * = 0
    ) :
        super(other.base()), depth(other.depth),
        first(other.first), last(other.last)
    {}

    Incrementable count() const
    {
        return depth;
    }

    template< class Other >
    bool is_compatible(Other const& other) const
    {
        return first == other.first && last == other.last;
    }

    reference dereference() const
    {
        assert(this->base() != last);
        return *this->base();
    }

    template< class F, class I >
    bool equal(cycle_iterator<F, I> const& other) const
    {
        assert(is_compatible(other));
        return depth == other.depth && this->base() == other.base();
    }

    void increment()
    {
        if (++this->base_reference() == last) {
            this->base_reference() = first;
            ++depth;
        }
    }

    void decrement()
    {

        if (this->base() == first) {
            this->base_reference() = last;
            --depth;
        }

        --this->base_reference();
    }

    void advance(difference_type n)
    {

        std::pair<difference_type, difference_type> const q_r =
            detail::positive_rem_div((this->base() - first) + n, last - first);
        assert(0 <= q_r.second);
        assert(q_r.second < last - first);

        this->base_reference() = first + q_r.second;
        depth += q_r.first;
    }

    template< class F, class I >
    difference_type distance_to(cycle_iterator<F, I> const& other) const
    {
        assert(is_compatible(other));

        return ((last - first) * (other.depth - depth)) + (other.base() - this->base());
    }
};


}

#endif
