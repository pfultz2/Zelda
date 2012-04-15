/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    back.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_TPL_BACK_H
#define ZELDA_GUARD_TPL_BACK_H

namespace zelda { namespace tpl {

template<class List>
struct back {};

template<template <typename...> class List, class Head>
struct back<List<Head>>
{
    typedef Head type;
};

template<template <typename...> class List, class Head, class... Args>
struct back<List<Head, Args...>>
{
    typedef typename back<List<Args...>>::type type;
};



}}

#endif
