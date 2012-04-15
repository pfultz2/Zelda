/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    pop_back.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_TPL_POP_BACK_H
#define ZELDA_GUARD_TPL_POP_BACK_H

#include <zelda/tpl/push_front.h>

namespace zelda { namespace tpl {

// (define (remove-last l) 
//   (if (null? l) '() 
//       (if (null? (cdr l)) '() 
//           (cons (car l) (remove-last (cdr l))))))

template<class TypeList>
struct pop_back {};

template<template <class...> class List>
struct pop_back<List<>>
{
    typedef List<> type;
};

template<template <class...> class List, class T>
struct pop_back<List<T>>
{
    typedef List<> type;
};

template<template <class...> class List, class T, class... Args>
struct pop_back<List<T, Args...>>
{
    typedef push_front<typename pop_back<List<Args...>>::type, T> type;
};



}}

#endif
