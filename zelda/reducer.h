/* 
 * File:   reducer.h
 * Author: pfultz
 *
 * Created on November 21, 2011, 4:40 PM
 */

#ifndef REDUCER_H
#define	REDUCER_H

#include "expression.h"
#include "requires.h"
#include "auto.h"

namespace zelda {


template<class Range, class Reducer>
typename boost::range_value<Range>::type reduce(Range& r, Reducer re)
{
    typedef typename boost::range_iterator<Range>::type iterator;
    typedef typename boost::range_value<Range>::type value_type;
    iterator first = boost::begin(r);
    iterator last = boost::end(r);
    if (first == last) value_type();
    value_type value = *first;
    first++;
    return std::accumulate(first, last, value, re);
}

namespace reducer
{


ZELDA_EXPR(x, y)(std::max(x, y)) max;
ZELDA_EXPR(x, y)(std::min(x, y)) min;
ZELDA_EXPR(x, y)(x+y) sum;

template<class Reducer>
struct kernel
{
    Reducer re;
    kernel(Reducer re) : re(re) {};
    template<class Range>
    ZELDA_FUNCTION_REQUIRES(is_range<Range>)
    (typename boost::range_value<Range>::type)
    operator()(Range& r)
    {
        return reduce(r, re);
    }
};
}

template<class Reducer>
reducer::kernel<Reducer> make_reducer(Reducer r)
{
    return reducer::kernel<Reducer>(r);
}

ZELDA_AUTO(max_element, make_reducer(reducer::max));
ZELDA_AUTO(min_element, make_reducer(reducer::min));
ZELDA_AUTO(sum_element, make_reducer(reducer::sum));


}

#endif	/* REDUCER_H */

