/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    if.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_TPL_IF_H
#define ZELDA_GUARD_TPL_IF_H

#include <zelda/tpl/lazy.h>

namespace zelda { namespace tpl {

namespace detail
{
struct no_else {};
}

template<bool C, class T = void, class E = detail::no_else>
struct if_c;

namespace detail
{

template<bool C, class T>
struct if_clause
{
    template<bool C1, class T1 = void, class E1 = no_else>
    struct else_if_c
    : zelda::tpl::if_c<C || C1, typename zelda::tpl::if_c<C, T, T1>::type, E1> {};

    template<class C1, class T1 = void, class E1 = no_else>
    struct else_if: else_if_c<C1::type::value, T1, E1> {};

    template<class E>
    struct else_: zelda::tpl::if_c<C, T, E> {};
};

}

template<class T, class E>
struct if_c<false, T, E>
: eval<E> {};

template<class T, class E>
struct if_c<true, T, E>
: eval<T> {};

template<class T>
struct if_c<false, T, detail::no_else>
: detail::if_clause<false, T> {};

template<class T>
struct if_c<true, T, detail::no_else> 
: detail::if_clause<true, T>, eval<T> {};

template<class C, class T = void, class E = detail::no_else>
struct if_: if_c<C::type::value, T, E> {};

}}

#endif
