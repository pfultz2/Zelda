/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    is_callable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <zelda/pp.h>
#include <zelda/static_assert.h>
#include <zelda/config.h>
#include <boost/mpl/bool.hpp>

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
    typedef private_type const &(*pointer_to_function)(ZELDA_PP_PARAMS(n, dont_care BOOST_PP_INTERCEPT)); \
    operator pointer_to_function() const; \
};
BOOST_PP_REPEAT(10, ZELDA_FUNWRAP_BUILDER, ~)
#undef ZELDA_FUNWRAP_BUILDER

// template<class Fun> struct funwrap<0, Fun> : Fun
// {
//     funwrap();
//     typedef private_type const &(*pointer_to_function)();
//     operator pointer_to_function() const;
// };
// template<class Fun> struct funwrap<1, Fun> : Fun
// {
//     funwrap();
//     typedef private_type const &(*pointer_to_function)( dont_care );
//     operator pointer_to_function() const;
// };



}

#define ZELDA_IS_CALLABLE_BUILDER(z, n, data) \
template<class Fun BOOST_PP_COMMA_IF(n) ZELDA_PP_PARAMS(n, class T)> \
struct is_callable<Fun(ZELDA_PP_PARAMS(n, T))> \
{ \
    static callable_detail::funwrap<n, Fun> & fun; \
    ZELDA_PP_GEN(n, static T, x, ; BOOST_PP_INTERCEPT) \
    static bool const value = \
    (\
        sizeof(callable_detail::no_type) == sizeof(callable_detail::is_private_type( (fun(ZELDA_PP_PARAMS(n, x)), 0) )) \
    ); \
    typedef boost::mpl::bool_<value> type; \
};
BOOST_PP_REPEAT(10, ZELDA_IS_CALLABLE_BUILDER, ~)
#undef ZELDA_IS_CALLABLE_BUILDER

// template<class Fun > struct is_callable<Fun()>
// {
//     static callable_detail::funwrap<0, Fun> & fun;
//     static bool const value = ( sizeof(callable_detail::no_type) == sizeof(callable_detail::is_private_type( (fun(), 0) )) );
//     typedef boost::mpl::bool_<value> type;
// };
// template<class Fun , class T0 > struct is_callable<Fun( T0 )>
// {
//     static callable_detail::funwrap<1, Fun> & fun;
//     static T0 x0 ;
//     static bool const value = ( sizeof(callable_detail::no_type) == sizeof(callable_detail::is_private_type( (fun( x0 ), 0) )) );
//     typedef boost::mpl::bool_<value> type;
// };

#else

template<class F, class... Args>
struct is_callable<F(Args...)>
{
    typedef char yes; 
    typedef long no; 
    template<int N> 
    struct selector {}; 

    template<class U> 
    static yes check( selector<sizeof( zelda::declval<U>()(zelda::declval<Args>()...) )> * ); 

    template<class U> 
    static no check(...); 
    
    static const bool value = sizeof(check<F>(0)) == sizeof(yes); 

    typedef boost::mpl::bool_<value> type; 
};

#endif

#ifdef ZELDA_TEST
struct is_callable_class
{
    template<class T>
    void operator()(T) const
    {
    }
};

typedef is_callable<is_callable_class(int)>::type is_callable_test;
static_assert(is_callable_test::value, "Not callable");
static_assert(is_callable<is_callable_class(const int&)>::type::value, "Not callable");

#endif


}