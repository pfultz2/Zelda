/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    forward.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FORWARD_H
#define	ZELDA_GUARD_FORWARD_H


#include <zelda/config.h>
#include <boost/type_traits.hpp>

namespace zelda {


#ifdef ZELDA_NO_RVALUE_REFS
#define ZELDA_FORWARD_REF(...) __VA_ARGS__ &

// template< class T >
// const T& forward( const typename boost::remove_reference<T>::type& t )
// {
//     return static_cast<const T&>(t);
// }

// template< class T >
// T& forward( typename boost::remove_reference<T>::type& t )
// {
//     return static_cast<T&>(t);
// }

#else
#define ZELDA_FORWARD_REF(...) __VA_ARGS__ &&

template< class T >
T&& forward( typename boost::remove_reference<T>::type&& t )
{
    return static_cast<T&&>(t);
}

#endif

template< class T >
ZELDA_FORWARD_REF(T) forward( typename boost::remove_reference<T>::type& t )
{
    return static_cast<ZELDA_FORWARD_REF(T)>(t);
}

}



#endif	/* ZELDA_GUARD_FORWARD_H */

