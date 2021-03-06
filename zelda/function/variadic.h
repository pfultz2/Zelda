/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    variadic.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_VARIADIC_H
#define ZELDA_GUARD_FUNCTION_VARIADIC_H

// @begin
// variadic
// ========
// 
// Description
// -----------
// 
// The `variadic` function adaptor converts the arguments to the function to a
// tuple or fusion sequence.
// 
// Synopsis
// --------
// 
//     template<class F>
//     variadic_adaptor<F> variadic(F f);
// 
// @end

#include <zelda/function/adaptor.h>
#include <zelda/function/perfect.h>
#include <zelda/function/result_of.h>
#include <zelda/function/detail/nullary_tr1_result_of.h>
#ifndef ZELDA_NO_VARIADIC_TEMPLATES
#include <tuple>
#include <boost/fusion/adapted/std_tuple.hpp>
#else
#include <boost/fusion/container/vector.hpp>
#endif


namespace zelda { 

// TODO: move to another file
template<class T>
struct remove_rvalue_reference
: boost::mpl::eval_if<boost::is_rvalue_reference<T>, boost::remove_reference<T>, boost::mpl::identity<T> >
{};

#ifdef ZELDA_TEST
// static_assert(!boost::is_rvalue_reference<typename remove_rvalue_reference<int&&>::type>::type::value, "Error");
static_assert(!boost::is_rvalue_reference<remove_rvalue_reference<int&>::type>::type::value, "Error");
static_assert(boost::is_lvalue_reference<remove_rvalue_reference<int&>::type>::type::value, "Error");
static_assert(!boost::is_rvalue_reference<remove_rvalue_reference<const int&>::type>::type::value, "Error");
static_assert(boost::is_lvalue_reference<remove_rvalue_reference<const int&>::type>::type::value, "Error");
// static_assert(boost::is_rvalue_reference<int&&>::type::value, "Error");
#endif

namespace detail {

// This is used to avoid rvalue references in fusion sequences, 
// since they don't work right now
template<class T>
struct tuple_reference
{
    typedef T type;
};
#ifndef ZELDA_NO_RVALUE_REFS
template<class T>
struct tuple_reference<T&&>
: tuple_reference<T>
{};
#endif
template<class T>
struct tuple_reference<T&>
{
    typedef T& type;
};

template<class T>
struct tuple_reference<const T>
{
    typedef const T& type;
};

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
template<class F>
struct variadic_adaptor_base : function_adaptor_base<F>
{
    typedef void zelda_is_callable_by_result_tag;
    variadic_adaptor_base() {}

    template<class X>
    variadic_adaptor_base(X x) : function_adaptor_base<F>(x)
    {}

    template<class X>
    struct result;

    template<class X, class... T>
    struct result<X(T...)>
    : zelda::result_of<F(std::tuple<typename tuple_reference<T>::type...>)> 
    {};

    template<class... T>
    typename zelda::result_of<F(std::tuple<typename tuple_reference<T&&>::type...>)>::type
    operator()(T && ... x) const
    {   
        return this->get_function()(std::tuple<typename tuple_reference<T&&>::type...>(std::forward<T>(x)...));
    }

};
#else

#define ZELDA_FUNCTION_VARIADIC_ADAPTOR(z, n, data) \
    template<class X BOOST_PP_COMMA_IF(n) ZELDA_PP_PARAMS_Z(z, n, class T)> \
    struct result<X(ZELDA_PP_PARAMS_Z(z, n, T))> \
    : zelda::result_of<F(boost::fusion::vector<ZELDA_PP_PARAMS_Z(z, n, typename tuple_reference<T, >::type BOOST_PP_INTERCEPT)>)> \
    {}; \
    ZELDA_PP_WHEN(n)(template<ZELDA_PP_PARAMS_Z(z, n, class T)>) \
    typename result<F(ZELDA_PP_PARAMS_Z(z, n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT))>::type \
    operator()(ZELDA_PP_PARAMS_Z(z, n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT, x)) const \
    { \
        return this->get_function()(boost::fusion::vector<ZELDA_PP_PARAMS_Z(z, n, typename tuple_reference<T, ZELDA_FORWARD_REF()>::type BOOST_PP_INTERCEPT)> \
            ( \
                ZELDA_PP_PARAMS_Z(z, n, zelda::forward<T, > BOOST_PP_INTERCEPT, (x)) \
            )); \
    }

template<class F, class Enable = void>
struct variadic_adaptor_base;

template<class F>
struct variadic_adaptor_base<F, ZELDA_CLASS_REQUIRES(exclude is_callable<F(boost::fusion::vector<>)>)> 
: function_adaptor_base<F>
{
    typedef void zelda_is_callable_by_result_tag;
    variadic_adaptor_base() {}

    template<class X>
    variadic_adaptor_base(X x) : function_adaptor_base<F>(x)
    {}

    template<class X>
    struct result;

BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_PARAMS_LIMIT, ZELDA_FUNCTION_VARIADIC_ADAPTOR, ~)

};

template<class F>
struct variadic_adaptor_base<F, ZELDA_CLASS_REQUIRES(is_callable<F(boost::fusion::vector<>)>)> 
: function_adaptor_base<F>
{
    typedef void zelda_is_callable_by_result_tag;
    variadic_adaptor_base() {}

    template<class X>
    variadic_adaptor_base(X x) : function_adaptor_base<F>(x)
    {}

    template<class X>
    struct result;

BOOST_PP_REPEAT_FROM_TO_1(0, ZELDA_PARAMS_LIMIT, ZELDA_FUNCTION_VARIADIC_ADAPTOR, ~)

};
#endif
}

template<class F>
struct variadic_adaptor : perfect_adaptor<detail::variadic_adaptor_base<F> >
{
    variadic_adaptor() {}

    template<class X>
    variadic_adaptor(X x) : perfect_adaptor<detail::variadic_adaptor_base<F> >(x)
    {}
};

template<class F>
variadic_adaptor<F> variadic(F f)
{
    return variadic_adaptor<F>(f);
}

}

ZELDA_NULLARY_TR1_RESULT_OF_N(1, zelda::variadic_adaptor)

#ifdef ZELDA_TEST
#include <zelda/test.h>
#include <zelda/function/detail/test.h>

typedef zelda::variadic_adaptor<tuple_class> vard_class;

ZELDA_TEST_CASE(variadic_test)
{
    ZELDA_TEST_EQUAL(3, vard_class()(2));
}
#endif

#endif
