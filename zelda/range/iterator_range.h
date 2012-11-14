/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    iterator_range.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ITERATOR_RANGE_H
#define ZELDA_GUARD_ITERATOR_RANGE_H

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace zelda {

template<class Iterator>
class iterator_range
{
	Iterator first, last;

public:

	typedef Iterator iterator;
	typedef Iterator const_iterator;

	iterator_range(Iterator first, Iterator last) : first(first), last(last)
	{}

	template<class Range>
	iterator_range(const Range& r) : first(boost::begin(r)), last(boost::end(r))
	{}

	template<class Range>
	iterator_range(Range& r) : first(boost::begin(r)), last(boost::end(r))
	{}

	Iterator begin() const
	{
		return first;
	}

	Iterator end() const
	{
		return last;
	}
};

template<class Iterator>
iterator_range<Iterator> make_iterator_range(Iterator first, Iterator last)
{
	return iterator_range<Iterator>(first, last);
}

template<class Range>
struct sub_range 
: iterator_range<typename boost::range_iterator<Range>::type>
{
	typedef iterator_range<typename boost::range_iterator<Range>::type> base;

	sub_range(typename base::iterator first, typename base::iterator last) : base(first, last)
	{}

	template<class X>
	sub_range(const X& r) : base(r)
	{}

	template<class X>
	sub_range(X& r) : base(r)
	{}
};

template<class Range>
sub_range<Range> make_sub_range(Range& r)
{
	return sub_range<Range>(r);
}

template<class Range>
sub_range<const Range> make_sub_range(const Range& r)
{
	return sub_range<const Range>(r);
}

}

#endif
