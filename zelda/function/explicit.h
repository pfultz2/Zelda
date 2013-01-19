/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    explicit.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_EXPLICIT_H
#define ZELDA_GUARD_FUNCTION_EXPLICIT_H

#include <zelda/function/adaptor.h>
#include <zelda/function/result_of.h>

#if !defined(ZELDA_NO_VARIADIC_TEMPLATES) && !defined(ZELDA_NO_RVALUE_REFS)
#include <zelda/function/detail/c11/perfect_facade.h>
#elif !defined(ZELDA_NO_RVALUE_REFS)
#include <zelda/function/detail/msvc/perfect_facade.h>
#else
#include <zelda/function/detail/c03/perfect_facade.h>
#endif

namespace zelda {

template<template <class> F>
struct explicit_
{
    ZELDA_PERFECT_FACADE_TPL(F<T_class>, F<T_class>(), class)
};

}

#endif
