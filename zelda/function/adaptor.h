/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    adaptor.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_ADAPTOR_H
#define ZELDA_GUARD_FUNCTION_ADAPTOR_H

#include <zelda/requires.h>
#include <zelda/tuple.h>
#include <zelda/result_of.h>

namespace zelda { 

//Optimization for stateless functions
template<class F, class Enable = void>
struct function_base
{
    F f;
    function_base() {};
    function_base(F f) : f(f) {};

    F get_function() const
    {
        return f;
    }
};

template<class F>
struct function_base<F, ZELDA_CLASS_REQUIRES(boost::is_empty<F>)>
{
    function_base() {};
    function_base(F) {};

    F get_function() const
    {
        return F();
    }
};

template<class A, class Enable = void>
struct function_adaptor_base
{
    A a;
    function_adaptor_base() {};
    function_adaptor_base(A a) : a(a) {};

    A& get_adaptor() const
    {
        return a;
    }
};

template<class A>
struct function_adaptor_base<A, ZELDA_CLASS_REQUIRES(boost::is_empty<A>)>
{
    function_adaptor_base() {};
    function_adaptor_base(A) {};

    A get_adaptor() const
    {
        return A();
    }
};

template<class Self, class A, class F>
struct function_adaptor : function_base<F>, function_adaptor_base<A>
{
    function_adaptor() {};
    function_adaptor(F f) : function_base<F>(f) {};
    function_adaptor(A a) : function_adaptor_base<A>(a) {};
    function_adaptor(F f, A a) : function_base<F>(f), function_adaptor_base<A>(a) {};

    template<class X, class Enable = void>
    struct result {};

    template<class X, class... T>
    struct result<X(T...)>
    : zelda::result_of<A(const Self, F, T...)> {};

    template<class... T>
    typename result<void(T...)>::type
    operator()(T && ... x) const
    {
        return this->get_adaptor()(static_cast<const Self&>(*this), this->get_function(), std::forward<T>(x)...);
    }
};

template<class Self, class A, class F>
struct function_variadic_adaptor : function_base<F>, function_adaptor_base<A>
{
    function_variadic_adaptor() {};
    function_variadic_adaptor(F f) : function_base<F>(f) {};
    function_variadic_adaptor(A a) : function_adaptor_base<A>(a) {};
    function_variadic_adaptor(F f, A a) : function_base<F>(f), function_adaptor_base<A>(a) {};

    template<class X, class Enable = void>
    struct result {};

    //TODO: Check if it is callable
    template<class X, class... T>
    struct result<X(T...)>
    : zelda::result_of<A(const Self, F, zelda::tuple<T&&...>)> {};


    template<class... T>
    typename result<void(T...)>::type
    operator()(T && ... x) const
    {
        return this->get_adaptor()(static_cast<const Self&>(*this), this->get_function(), zelda::forward_as_tuple(std::forward<T>(x)...));
    }

};

#define DETAIL_ZELDA_FUNCTION_MAKE_ADAPTOR(name, adaptor_type, adaptor_base) \
template<class F> \
struct name ## _adaptor : adaptor_type<name ## _adaptor<F>, adaptor_base, F> \
{ \
    typedef adaptor_type<name ## _adaptor<F>, adaptor_base, F> base;\
    name ## _adaptor() {}; \
    name ## _adaptor(F f) : base(f) {}; \
}; \
template<class F> \
name ## _adaptor<F> name(F f) { return name ## _adaptor<F>(f); }


#define ZELDA_FUNCTION_MAKE_SIMPLE_ADAPTOR(name) DETAIL_ZELDA_FUNCTION_MAKE_ADAPTOR(name, function_adaptor, name ## _adaptor_base)
#define ZELDA_FUNCTION_MAKE_ADAPTOR(name) DETAIL_ZELDA_FUNCTION_MAKE_ADAPTOR(name, function_variadic_adaptor, name ## _adaptor_base)



}

#endif
