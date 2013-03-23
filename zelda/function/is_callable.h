/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    is_callable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_IS_CALLABLE_H
#define ZELDA_GUARD_IS_CALLABLE_H

#include <zelda/config.h>
#include <zelda/pp.h>
#include <zelda/typeof.h>
#include <zelda/forward.h>
#include <zelda/static_assert.h>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/function_types/is_function.hpp>
#include <boost/function_types/is_function_pointer.hpp>

#include <boost/phoenix/core/actor.hpp>
#include <boost/phoenix/core/is_nullary.hpp>

#ifdef ZELDA_TEST
#include <zelda/function/placeholders.h>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/statement/if.hpp>
#endif

#ifndef ZELDA_IS_CALLABLE_LIMIT
#define ZELDA_IS_CALLABLE_LIMIT 16
#endif

namespace zelda {

template<class F>
struct is_callable;


#if defined(ZELDA_NO_VARIADIC_TEMPLATES) || defined(ZELDA_NO_EXPRESSION_SFINAE)
// is_callable from Eric Niebler
namespace callable_detail {
struct dont_care
{
    dont_care(...);
};

struct private_type
{
    private_type const &operator,(int) const;
};

typedef char yes_type;      // sizeof(yes_type) == 1
typedef char (&no_type)[2]; // sizeof(no_type)  == 2

template<typename T>
no_type is_private_type(T const &);

yes_type is_private_type(private_type const &);

template<int N, class Fun>
struct funwrap;


#define ZELDA_FUNWRAP_BUILDER(z, n, data) \
template<class Fun> \
struct funwrap<n, Fun> : Fun \
{ \
    funwrap(); \
    typedef private_type const &(*pointer_to_function)(ZELDA_PP_PARAMS_Z(z, n, dont_care BOOST_PP_INTERCEPT)); \
    operator pointer_to_function() const; \
};
BOOST_PP_REPEAT_1(16, ZELDA_FUNWRAP_BUILDER, ~)
#undef ZELDA_FUNWRAP_BUILDER

template<class F, class G>
struct is_callable_fp
: boost::mpl::bool_<false>
{};
#define ZELDA_IS_CALLABLE_FP(z, n, data) \
template<class Fun BOOST_PP_COMMA_IF(n) ZELDA_PP_PARAMS_Z(z, n, class T), \
         class Res BOOST_PP_COMMA_IF(n) ZELDA_PP_PARAMS_Z(z, n, class U)> \
struct is_callable_fp<Fun(ZELDA_PP_PARAMS_Z(z, n, T)), Res(*)(ZELDA_PP_PARAMS_Z(z, n, U))> \
{ \
    static bool const value = \
        (true ZELDA_PP_GEN_Z(z, n, && boost::is_convertible<T, ZELDA_PP_INTERCEPT_COMMA, U, >::value BOOST_PP_INTERCEPT)); \
    typedef boost::mpl::bool_<value> type; \
};
BOOST_PP_REPEAT_1(ZELDA_IS_CALLABLE_LIMIT, ZELDA_IS_CALLABLE_FP, ~)

template<class F>
struct is_callable_fo;
#define ZELDA_IS_CALLABLE_FO(z, n, data) \
template<class Fun BOOST_PP_COMMA_IF(n) ZELDA_PP_PARAMS_Z(z, n, class T)> \
struct is_callable_fo<Fun(ZELDA_PP_PARAMS_Z(z, n, T))> \
{ \
    static funwrap<n, Fun> & fun; \
    ZELDA_PP_GEN_Z(z, n, static typename add_forward_reference<T, >::type BOOST_PP_INTERCEPT, x, ; BOOST_PP_INTERCEPT) \
    static bool const value = \
    (\
        sizeof(no_type) == sizeof(is_private_type( (fun(ZELDA_PP_PARAMS_Z(z, n, x)), 0) )) \
    ); \
    typedef boost::mpl::bool_<value> type; \
};
BOOST_PP_REPEAT_1(ZELDA_IS_CALLABLE_LIMIT, ZELDA_IS_CALLABLE_FO, ~)

}

#define ZELDA_IS_CALLABLE_BUILDER(z, n, data) \
template<class Fun BOOST_PP_COMMA_IF(n) ZELDA_PP_PARAMS_Z(z, n, class T)> \
struct is_callable<Fun(ZELDA_PP_PARAMS_Z(z, n, T))> \
: boost::mpl::eval_if<boost::function_types::is_function_pointer<Fun>, \
                    callable_detail::is_callable_fp<Fun(ZELDA_PP_PARAMS_Z(z, n, T)), Fun>, \
                    callable_detail::is_callable_fo<Fun(ZELDA_PP_PARAMS_Z(z, n, T))> >::type \
{};
BOOST_PP_REPEAT_1(ZELDA_IS_CALLABLE_LIMIT, ZELDA_IS_CALLABLE_BUILDER, ~)

#else

template<class F, class... Args>
struct is_callable<F(Args...)>
{
    typedef char yes;
    typedef char (&no)[2];
    template<class T> 
    struct selector {}; 

    template<class U> 
    static yes check( selector<decltype( zelda::declval<U>()(zelda::declval<Args>()...) )> * ); 

    template<class U> 
    static no check(...); 
    
    static const bool value = sizeof(check<F>(0)) == sizeof(yes); 

    typedef boost::mpl::bool_<value> type; 
};

#endif

// Workaround for Boost.Phoenix
template<class Expr>
struct is_callable<boost::phoenix::actor<Expr>()>
: boost::phoenix::result_of::is_nullary<Expr>
{};

#ifdef ZELDA_TEST
struct is_callable_class
{
    void operator()(int) const
    {
    }
};
struct callable_test_param {};

void is_callable_function(int)
{
}
// using zelda::ph::_1;
template<class F>
void phoenix_not_nullary_check(F)
{
    static_assert(!is_callable<F()>::type::value, "Callable failed");
}

inline void callable_test()
{
    phoenix_not_nullary_check(zelda::ph::_1 + 1);
}
// typedef is_callable<is_callable_class(int)>::type is_callable_test;
static_assert(is_callable<is_callable_class(int)>::type::value, "Not callable");
static_assert(is_callable<is_callable_class(const int&)>::type::value, "Not callable");
static_assert(not is_callable<is_callable_class(callable_test_param)>::type::value, "callable failed");
static_assert(not is_callable<is_callable_class()>::type::value, "callable failed");
static_assert(not is_callable<is_callable_class(int, int)>::type::value, "callable failed");

typedef void (*is_callable_function_pointer)(int);
static_assert(boost::function_types::is_function_pointer<is_callable_function_pointer>::value, "Not callable");
// typedef boost::mpl::identity<is_callable_function_pointer>::type is_callable_function_type;
// typedef is_callable<is_callable_function_pointer(int)>::type is_callable_test;
static_assert(is_callable<is_callable_function_pointer(int)>::type::value, "Not callable");
static_assert(is_callable<is_callable_function_pointer(const int&)>::type::value, "Not callable");
static_assert(not is_callable<is_callable_function_pointer(callable_test_param)>::type::value, "callable failed");
static_assert(not is_callable<is_callable_function_pointer()>::type::value, "callable failed");
static_assert(not is_callable<is_callable_function_pointer(int, int)>::type::value, "callable failed");

#endif


}

#endif