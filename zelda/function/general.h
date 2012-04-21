/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    general.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_GENERAL_H
#define ZELDA_GUARD_FUNCTION_GENERAL_H

#include <zelda/function/adaptor.h>
#include <zelda/function/lazy.h>

namespace zelda { 


//general
namespace details{
template<class... T>
struct is_phoenix_expression
: zelda::tpl::or_<T...> {};
}
template<class F>
struct general_adaptor : function_base<F>
{
    general_adaptor() {};
    general_adaptor(F f) : function_base<F>(f) {};

    template<class X, class E = void>
    struct result {};

    template<class X, class... Args>
    struct result<X(Args...), ZELDA_CLASS_REQUIRES(zelda::details::is_phoenix_expression<Args...>)>
    {
        typedef ZELDA_XTYPEOF(lazy(zelda::declval<F>())(zelda::declval<Args>()...)) type;
    };

    template<class X, class... Args>
    struct result<X(Args...), ZELDA_CLASS_REQUIRES(not zelda::details::is_phoenix_expression<Args...>)>
    : zelda::result_of<X(Args...)> {};

    template<class... Args>
    ZELDA_FUNCTION_REQUIRES(zelda::details::is_phoenix_expression<Args...>)
    (typename result<void(Args&&...)>::type) operator()(Args&&... args) const
    {
        return lazy(this->get_function())(std::forward<Args>(args)...);
    }

    template<class... Args>
    ZELDA_FUNCTION_REQUIRES(not zelda::details::is_phoenix_expression<Args...>)
    (typename result<void(Args&&...)>::type) operator()(Args&&... args) const
    {
        return this->get_function()(std::forward<Args>(args)...);
    }

};



}

#endif
