/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    zip.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ALGORITHM_ZIP_H
#define ZELDA_GUARD_ALGORITHM_ZIP_H

#include <zelda/function/builder.h>
#include <zelda/traits.h>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <zelda/typeof.h>
#include <zelda/function/invoke.h>
#include <zelda/range/iterator_range.h>

#include <boost/iterator/zip_iterator.hpp>
#include <boost/fusion/algorithm/transformation/zip.hpp>

namespace zelda { 

namespace detail {
#if !defined(ZELDA_NO_VARIADIC_TEMPLATES) && !defined(ZELDA_NO_TRAILING_RETURN)
template<class T, int ...N>
auto make_boost_tuple_impl(T && t, seq<N...>) ZELDA_RETURNS(boost::make_tuple(boost::fusion::at_c<N>(zelda::forward<T>(t))...));

template<class F, class Sequence>
auto make_boost_tuple(Sequence && t) ZELDA_RETURNS
(
    detail::make_boost_tuple_impl(std::forward<Sequence>(t), typename detail::sequence_gens<Sequence>::type() ) 
);

struct fusion_zip
{
    template<class... T>
    auto operator()(const T & ...x) ZELDA_RETURNS(boost::fusion::zip(x...));
};

// template<class... Range>
// auto zip(Range && ...r) ZELDA_RETURNS
// (
//     make_iterator_range
//     (
//         boost::make_zip_iterator(boost::make_tuple(boost::begin(r)...)),
//         boost::make_zip_iterator(boost::make_tuple(boost::end(r)...))
//     )
// );
#else

#endif

ZELDA_FUNCTION_CLASS((begin_select)(auto r)(boost::begin(r)))
ZELDA_FUNCTION_CLASS((end_select)(auto r)(boost::end(r)))

//ZELDA_FUNCTION_CLASS((zip_impl)(t))

}



}

#endif
