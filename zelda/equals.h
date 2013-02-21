/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    equals.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ZELDA_EQUALS_H
#define ZELDA_GUARD_ZELDA_EQUALS_H

#include <zelda/requires.h>
#include <zelda/traits.h>

namespace zelda { 

template<class T, class U>
ZELDA_FUNCTION_REQUIRES(exclude is_pair<T>, exclude is_pair<U>, exclude is_range<T>, exclude is_range<U>)
(bool) equals(const T& x, const U& y);

template<class Range1, class Range2>
ZELDA_FUNCTION_REQUIRES(is_range<Range1>, is_range<Range2>)
(bool) equals(const Range1& r1, const Range2& r2);

template<class Pair1, class Pair2>
ZELDA_FUNCTION_REQUIRES(is_pair<Pair1>, is_pair<Pair2>, exclude is_range<Pair1>, exclude is_range<Pair2>)
(bool) equals(const Pair1& p1, const Pair2& p2);


namespace detail {

template <class InputIterator1, class InputIterator2, class Predicate>
bool equal ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Predicate pred )
{
  while ((first1 != last1) && (first2 != last2)) 
  {
    if (!pred(*first1,*first2))
      return false;
    ++first1; ++first2;
  }
  return ((first1 == last1) && (first2 == last2));
}

struct equals_predicate
{
    template<class T>
    bool operator()(const T& x, const T& y) const
    {
        return equals(x, y);
    }
};

}

template<class T, class U>
ZELDA_FUNCTION_REQUIRES(exclude is_pair<T>, exclude is_pair<U>, exclude is_range<T>, exclude is_range<U>)
(bool) equals(const T& x, const U& y)
{
    return x == y;
}

template<class Range1, class Range2>
ZELDA_FUNCTION_REQUIRES(is_range<Range1>, is_range<Range2>)
(bool) equals(const Range1& r1, const Range2& r2)
{
    return detail::equal(boost::begin(r1), boost::end(r1), boost::begin(r2), boost::end(r2), detail::equals_predicate());
}

template<class Pair1, class Pair2>
ZELDA_FUNCTION_REQUIRES(is_pair<Pair1>, is_pair<Pair2>, exclude is_range<Pair1>, exclude is_range<Pair2>)
(bool) equals(const Pair1& p1, const Pair2& p2)
{
    return equals(p2.first, p1.first) && equals(p2.second, p2.second);
}

}

#endif
