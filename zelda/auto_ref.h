/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    auto_ref.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ZELDA_AUTO_REF_H
#define ZELDA_GUARD_ZELDA_AUTO_REF_H

#include <zelda/config.h>
#include <zelda/forward.h>
#include <boost/ref.hpp>

namespace zelda { 

#ifdef ZELDA_NO_RVALUE_REFS
template<class T> 
boost::reference_wrapper<T> auto_ref(T & t)
{ 
    return boost::reference_wrapper<T>(t);
}

template<class T> 
T auto_ref(T t)
{ 
    return t;
}
#else

namespace detail {

template<class T>
struct auto_ref_type
{
    typedef T type;
};

template<class T>
struct auto_ref_type<T&&>
: auto_ref_type<T>
{};

template<class T>
struct auto_ref_type<T&>
{
    typedef boost::reference_wrapper<T> type;
};

template<class T>
struct auto_ref_type<const T&>
{
    typedef boost::reference_wrapper<const T> type;
};

}

template<class T>
typename detail::auto_ref_type<T>::type auto_ref(T&& x)
{
    return typename detail::auto_ref_type<T>::type(zelda::forward<T>(x));
}
#endif

}

#endif
