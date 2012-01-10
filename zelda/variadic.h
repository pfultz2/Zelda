/* 
 * File:   variadic.h
 * Author: pfultz
 *
 * Created on November 19, 2011, 4:59 PM
 */

#ifndef VARIADIC_H
#define	VARIADIC_H

#include "pp.h"
#include <boost/mpl/vector.hpp>
#include <boost/mpl/remove.hpp>

#define ZELDA_VARIADIC_PACK(x) BOOST_MPL_PP_DEFAULT_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, x, boost::mpl::na)
#define ZELDA_VARIADIC_UNPACK_VECTOR(x) boost::mpl::vector< BOOST_MPL_PP_PARAMS(BOOST_MPL_LIMIT_VECTOR_SIZE, x) >
#define ZELDA_VARIADIC_UNPACK_TUPLE(x) typename zelda::tuple_factory< ZELDA_VARIADIC_VECTOR_UNPACK(x) >::type


namespace zelda {

namespace details {
template<int N, class Sequence>
struct tuple_factory_impl;

#define ZELDA_DETAIL_VARIADIC_TUPLE_FACTORY_EACH(z, n, data) typename boost::mpl::at_c<Sequence, n>::type
#define ZELDA_DETAIL_VARIADIC_TUPLE_FACTORY(z, n, data) \
template<class Sequence> \
struct tuple_factory_impl<n, Sequence > \
{ \
typedef boost::tuple<BOOST_PP_ENUM(n, ZELDA_DETAIL_VARIADIC_TUPLE_FACTORY_EACH, data)> type; \
}; \

BOOST_PP_REPEAT(10, ZELDA_DETAIL_VARIADIC_TUPLE_FACTORY, data)
}

template<class Sequence>
struct tuple_factory : details::tuple_factory_impl<boost::mpl::size<Sequence>::value, Sequence>
{
};

        
}

#endif	/* VARIADIC_H */

