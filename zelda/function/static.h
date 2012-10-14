/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    static.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_STATIC_H
#define ZELDA_GUARD_FUNCTION_STATIC_H

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

template<class F>
struct static_ : detail::perfect_facade<static_<F>, F>
{
    F get_function() const
    {
        return F();
    }
};

}

#endif
