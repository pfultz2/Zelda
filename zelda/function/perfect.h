/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    perfect.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_PERFECT_H
#define ZELDA_GUARD_FUNCTION_PERFECT_H

#include <zelda/function/adaptor.h>
#include <zelda/result_of.h>

#if !defined(ZELDA_NO_VARIADIC_TEMPLATES) && !defined(ZELDA_NO_RVALUE_REFS)
#include <zelda/function/detail/c11/perfect_facade.h>
#elif !defined(ZELDA_NO_RVALUE_REFS)
#include <zelda/function/detail/msvc/perfect_facade.h>
#else
#include <zelda/function/detail/c03/perfect_facade.h>
#endif

namespace zelda {


// TODO: Add support for forwarding nullary functions as well
template<class F>
struct perfect_adaptor : function_adaptor_base<F>
{
    perfect_adaptor() {}

    template<class X>
    perfect_adaptor(X x) : function_adaptor_base<F>(x)
    {}

    template<class S>
    struct result
    : zelda::result_of<S, F>
    {};

    ZELDA_PERFECT_FACADE(F, this->get_function())
};

template<class F>
perfect_adaptor<F> perfect(F f)
{
    return perfect_adaptor<F>(f);
}

}

#endif
