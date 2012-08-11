/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    result_of.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_POLY_H
#define ZELDA_GUARD_POLY_H

#include <zelda/forward.h>

namespace zelda {

namespace details {
    struct poly_na {};
}

#if !defined(ZELDA_NO_VARIADIC_TEMPLATES) && !defined(ZELDA_NO_RVALUE_REFS)


#define ZELDA_DETAIL_POLY_ADAPTOR_BASE_FUNC() \
    template<class X, class... T> \
    struct result<X(T...), ZELDA_CLASS_REQUIRES(zelda::is_callable<F(T...)>)> \
    : zelda::result_of<F(T...)> {}; \
    template<class X, class... T> \
    struct result<X(T...), ZELDA_CLASS_REQUIRES(exclude zelda::is_callable<F(T...)>)> \
    : zelda::result_of<BaseFunction(T...)> {}; \
    \
    template<class... T> \
    ZELDA_FUNCTION_REQUIRES(zelda::is_callable<F(T&&...)>) \
    (typename zelda::result_of<F(T&&...)>::type) \
    operator()(T &&... x) \
    { \
        this->get_function()(zelda::forward<T>(x)...); \
    } \
    \
    template<class... T> \
    ZELDA_FUNCTION_REQUIRES(exclude zelda::is_callable<F(T&&...)>) \
    (typename zelda::result_of<BaseFunction(T&&...)>::type) \
    operator()(T &&... x) \
    { \
        this->get_base_function()(zelda::forward<T>(x)...); \
    }

#define ZELDA_DETAIL_POLY_ADAPTOR_BASE_SINGLE_FUNC() \
    template<class X, class... T> \
    struct result<X(T...), ZELDA_CLASS_REQUIRES(zelda::is_callable<F(T...)>)> \
    : zelda::result_of<F(T...)> {}; \
    \
    template<class... T> \
    ZELDA_FUNCTION_REQUIRES(zelda::is_callable<F(T&&...)>) \
    (typename zelda::result_of<F(T&&...)>::type) \
    operator()(T &&... x) \
    { \
        this->get_function()(zelda::forward<T>(x)...); \
    } 


#else

#define ZELDA_DETAIL_POLY_ADAPTOR_BASE_FUNC_EACH(z, n, data) \
    template<class X, ZELDA_PP_PARAMS(n, class T)> \
    struct result<X(ZELDA_PP_PARAMS(n, T)), ZELDA_CLASS_REQUIRES(zelda::is_callable<F(ZELDA_PP_PARAMS(n, T))>)> \
    : zelda::result_of<F(ZELDA_PP_PARAMS(n, T))> {}; \
    template<class X, ZELDA_PP_PARAMS(n, class T)> \
    struct result<X(ZELDA_PP_PARAMS(n, T)), ZELDA_CLASS_REQUIRES(exclude zelda::is_callable<F(ZELDA_PP_PARAMS(n, T))>)> \
    : zelda::result_of<BaseFunction(ZELDA_PP_PARAMS(n, T))> {}; \
    \
    template<ZELDA_PP_PARAMS(n, class T)> \
    ZELDA_FUNCTION_REQUIRES(zelda::is_callable<F(ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT))>) \
    (typename zelda::result_of<F(ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT))>::type) \
    operator()(ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT, x)) \
    { \
        this->get_function()(ZELDA_FORWARD_FUNCTION(n, T, x)); \
    } \
    \
    template<class... T> \
    ZELDA_FUNCTION_REQUIRES(exclude zelda::is_callable<F(ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT))>) \
    (typename zelda::result_of<BaseFunction(ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT))>::type) \
    operator()(ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT, x)) \
    { \
        this->get_base_function()(ZELDA_FORWARD_FUNCTION(n, T, x)); \
    }

#define ZELDA_DETAIL_POLY_ADAPTOR_BASE_FUNC() \
    BOOST_PP_REPEAT_FROM_TO(1, 7, ZELDA_DETAIL_POLY_ADAPTOR_BASE_FUNC_EACH, ~)

#define ZELDA_DETAIL_POLY_ADAPTOR_BASE_SINGLE_FUNC_EACH(z, n, data) \
    template<class X, ZELDA_PP_PARAMS(n, class T)> \
    struct result<X(ZELDA_PP_PARAMS(n, T)), ZELDA_CLASS_REQUIRES(zelda::is_callable<F(ZELDA_PP_PARAMS(n, T))>)> \
    : zelda::result_of<F(ZELDA_PP_PARAMS(n, T))> {}; \
    \
    template<ZELDA_PP_PARAMS(n, class T)> \
    ZELDA_FUNCTION_REQUIRES(zelda::is_callable<F(ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT))>) \
    (typename zelda::result_of<F(ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT))>::type) \
    operator()(ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT, x)) \
    { \
        this->get_function()(ZELDA_FORWARD_FUNCTION(n, T, x)); \
    }

#define ZELDA_DETAIL_POLY_ADAPTOR_BASE_SINGLE_FUNC() \
    BOOST_PP_REPEAT_FROM_TO(1, 7, ZELDA_DETAIL_POLY_ADAPTOR_BASE_SINGLE_FUNC_EACH, ~)



#endif

template<class F, class BaseFunction = void, class Enable = void>
struct poly_adaptor_base
{
    F f;
    BaseFunction bf;

    poly_adaptor_base() {}
    poly_adaptor_base(F f, BaseFunction bf) : f(f), bf(bf) {}

    const F& get_function() const
    {
        return f;
    }

    const BaseFunction& get_base_function() const
    {
        return bf;
    }

    template<class X, class ResultEnable = void>
    struct result;

    ZELDA_DETAIL_POLY_ADAPTOR_BASE_FUNC()
};

template<class F, class BaseFunction>
struct poly_adaptor_base<F, BaseFunction, ZELDA_CLASS_REQUIRES(boost::is_empty<F>, boost::is_empty<BaseFunction>)>
{

    F get_function() const
    {
        return F();
    }

    BaseFunction get_base_function() const
    {
        return BaseFunction();
    }

    template<class X, class ResultEnable = void>
    struct result;

    ZELDA_DETAIL_POLY_ADAPTOR_BASE_FUNC()
};

// For just one function
template<class F>
struct poly_adaptor_base<F, void, ZELDA_CLASS_REQUIRES(exclude boost::is_empty<F>)>
{
    F f;

    poly_adaptor_base() {}
    poly_adaptor_base(F f) : f(f) {}

    const F& get_function() const
    {
        return f;
    }

    template<class X, class ResultEnable = void>
    struct result;

    ZELDA_DETAIL_POLY_ADAPTOR_BASE_SINGLE_FUNC()
};

template<class F>
struct poly_adaptor_base<F, void, ZELDA_CLASS_REQUIRES(boost::is_empty<F>)>
{

    F get_function() const
    {
        return F();
    }

    template<class X, class ResultEnable = void>
    struct result;

    ZELDA_DETAIL_POLY_ADAPTOR_BASE_SINGLE_FUNC()
};





template<class F, ZELDA_PP_PARAMS(10, class F, = details::poly_na BOOST_PP_INTERCEPT)>
struct poly_adaptor_base_type
{
    typedef poly_adaptor_base<F, typename poly_adaptor_base_type<ZELDA_PP_PARAMS(10, F)>::type> type;
};

template<class F>
struct poly_adaptor_base_type<F, ZELDA_PP_PARAMS(10, details::poly_na BOOST_PP_INTERCEPT)>
{
    typedef poly_adaptor_base<F> type;
};

template<class F>
ZELDA_FUNCTION_REQUIRES(exclude boost::is_empty<poly_adaptor_base<F> >)
(poly_adaptor_base<F>) make_poly_adaptor_base(F f)
{
    return poly_adaptor_base<F>(f);
}

template<class F>
ZELDA_FUNCTION_REQUIRES(boost::is_empty<poly_adaptor_base<F> >)
(poly_adaptor_base<F>) make_poly_adaptor_base(F f)
{
    return poly_adaptor_base<F>();
}

#define ZELDA_DETAIL_POLY_MAKE_ADAPTOR_BASE_EACH(z, n, data) \
template<class F, ZELDA_PP_PARAMS(n, class F)> \
ZELDA_FUNCTION_REQUIRES(exclude boost::is_empty<typename poly_adaptor_base_type<F, ZELDA_PP_PARAMS(n, F)>::type>) \
(typename poly_adaptor_base_type<F, ZELDA_PP_PARAMS(n, F)>::type) \
make_poly_adaptor_base(F f, ZELDA_PP_PARAMS(n, F, f)) \
{ \
    return typename poly_adaptor_base_type<F, ZELDA_PP_PARAMS(n, F)>::type(f, make_poly_adaptor_base(ZELDA_PP_PARAMS(n, f))); \
} \
template<class F, ZELDA_PP_PARAMS(n, class F)> \
ZELDA_FUNCTION_REQUIRES(boost::is_empty<typename poly_adaptor_base_type<F, ZELDA_PP_PARAMS(n, F)>::type>) \
(typename poly_adaptor_base_type<F, ZELDA_PP_PARAMS(n, F)>::type) \
make_poly_adaptor_base(F f, ZELDA_PP_PARAMS(n, F, f)) \
{ \
    return typename poly_adaptor_base_type<F, ZELDA_PP_PARAMS(n, F)>::type(); \
}


// template<class Base, class F, class BF>
// ZELDA_FUNCTION_REQUIRES(exclude boost::is_empty<Base>)(void) 
// set_poly_adaptor_base(Base& b, F f, BF bf)
// {
//     b.f = f;
//     b.bf = bf;
// }

// template<class Base, class F, class BF>
// ZELDA_FUNCTION_REQUIRES(boost::is_empty<Base>)(void) 
// set_poly_adaptor_base(Base& b, F f, BF bf)
// {
// }

// #define ZELDA_POLY_SET_POLY_ADAPTOR_BASE(z, i, data) \
// template<class Base, class F, class BF>


// template<class F, class BF>
// typename poly_adaptor_base_type<F, BF>::type
// make_poly_adaptor_base(F f, BF bf)
// {
//     typename poly_adaptor_base_type<F, BF>::type result;

//     return result;
// }

#define ZELDA_DETAIL_POLY_ADAPTOR_BASE typename poly_adaptor_base_type<F, ZELDA_PP_PARAMS(10, F)>::type 

template<class F, ZELDA_PP_PARAMS(10, class F, = details::poly_na BOOST_PP_INTERCEPT), class Enable = void>
struct poly_adaptor
{
    typedef ZELDA_DETAIL_POLY_ADAPTOR_BASE base;
    base f;
    poly_adaptor() {}
    poly_adaptor(base f) : f(f) {}
    const base& get_function() const
    {
        return f;
    }
    template<class X>
    struct result;
    ZELDA_FORWARD_CALL_OPERATOR_ZR(1, 1, base, f)
    
};
template<class F, ZELDA_PP_PARAMS(10, class F)>
struct poly_adaptor<F, ZELDA_PP_PARAMS(10, F), ZELDA_CLASS_REQUIRES(boost::is_empty<ZELDA_DETAIL_POLY_ADAPTOR_BASE >)> 
{ 
    typedef ZELDA_DETAIL_POLY_ADAPTOR_BASE base;
    base get_function() const
    {
        return base();
    }
    template<class X>
    struct result;
    ZELDA_FORWARD_CALL_OPERATOR_ZR(1, 1, base, base())
};

// TODO: add a poly function


}

#endif
