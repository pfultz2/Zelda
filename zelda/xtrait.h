/* 
 * File:   xtrait.h
 * Author: pfultz
 *
 * Created on December 23, 2011, 12:21 PM
 */

#ifndef XTRAIT_H
#define	XTRAIT_H

#include <boost/type_traits.hpp>

template<class X, class Y>
struct xtrait
{
    template<class T>
    T value_of();
    struct no {};
    
    template<class X, class Y>
    auto check(X x, Y, y) -> decltype(x + y);
    
    template<class X, class Y>
    no check(...);
    
    typedef boost::is_same<decltype(check(value_of<X>(), value_of<Y>)), no>::type not_type;
    static const bool value = not not_type::value;
    typedef boost::mpl::bool_<value> type;
};

#endif	/* XTRAIT_H */

