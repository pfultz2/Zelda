/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    fold.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_TPL_FOLD_H
#define ZELDA_GUARD_TPL_FOLD_H

namespace zelda { namespace tpl {

// foldl f z []     = z
// foldl f z (x:xs) = foldl f (f z x) xs

template<class TypeList, class Z, class MF>
struct fold {};


template<template <class...> class List, class Z, class MF>
struct fold<List<>, Z, MF>
{
    typedef Z type;
};

template<template <class...> class List, class X, class Z, class MF, class... Args>
struct fold<List<X, Args...>, Z, MF>
: fold<List<Args...>, typename MF::template apply<Z, X>::type, MF> {};

// foldr f z []     = z
// foldr f z (x:xs) = f x (foldr f z xs)

}}

#endif
