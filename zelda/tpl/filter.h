/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    filter.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_TPL_FILTER_H
#define ZELDA_GUARD_TPL_FILTER_H

#include <zelda/tpl/if.h>
#include <zelda/tpl/push_front.h>

namespace zelda { namespace tpl {


// filter :: (a -> Bool) -> [a] -> [a]
// filter _ []                 = []
// filter p (x:xs) | p x       = x : filter p xs
//                 | otherwise = filter p xs

namespace details{

template<class P, class X, class List>
struct push_front_filter
: if_<P, lazy< push_front<typename List::type, X> >, typename List::type>
{};
}

template<class TypeList, class P>
struct filter {};

template<template <class...> class List, class P>
struct filter<List<>, P>
{
    typedef List<> type;
};

template<template <class...> class List, class X, class P, class... Xs>
struct filter<List<X, Xs...>, P>
: details::push_front_filter<typename P::template apply<X>, X, filter<List<Xs...>, P> > 
{};

}}

#endif
