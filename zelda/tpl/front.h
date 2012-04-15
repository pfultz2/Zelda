/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    front.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_TPL_FRONT_H
#define ZELDA_GUARD_TPL_FRONT_H

namespace zelda { namespace tpl {

template<class TypeList>
struct front {};

template<template <class...> class List, class T, class... Args>
struct front<List<T, Args...>> 
{
    typedef T type;
};

}}

#endif
