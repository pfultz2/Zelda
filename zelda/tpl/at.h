/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    at.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_TPL_AT_H
#define ZELDA_GUARD_TPL_AT_H

namespace zelda { namespace tpl {

template<class TypeList, int N, int M = 0>
struct at_c {};

template<template <class...> class List, class X, int N, int M, class... Xs>
struct at_c<List<X, Xs...>, N, M>
: at_c<List<Xs...>, N, M+1> {};

template<template <class...> class List, class X, int N, class... Xs>
struct at_c<List<X, Xs...>, N, N>
{
    typedef X type;
};

template<class TypeList, class N>
struct at
: at_c<TypeList, N::type::value> {};

}}

#endif
