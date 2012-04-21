/* 
 * File:   introspection.h
 * Author: pfultz
 *
 * Created on November 17, 2011, 9:00 PM
 */

#ifndef ZELDA_INTROSPECTION_H
#define	ZELDA_INTROSPECTION_H

#include "pp.h"
#include "requires.h"
#include "typeof.h"
#include <zelda/tpl/integral_constant.h>
#include <zelda/tpl/if.h>
#include <zelda/tpl/or.h>
#include <zelda/tpl/and.h>
#include <boost/type_traits.hpp>


namespace zelda{ namespace introspection {

struct void_ {};


template<class From, class To>
struct check_type 
: boost::is_convertible<From, To> {};

template<class From>
struct check_type<From, void>
: zelda::tpl::bool_<true> {};

template<class From>
struct check_type<From, void_>
: zelda::tpl::bool_<true> {};
}
}


#define ZELDA_HAS_MEMBER(...) \
DETAIL_ZELDA_HAS_MEMBER(PP_HEAD(__VA_ARGS__), DETAIL_ZELDA_TRAIT_NAME(__VA_ARGS__))

#define ZELDA_HAS_STATIC_MEMBER(...) \
DETAIL_ZELDA_HAS_STATIC_MEMBER(PP_HEAD(__VA_ARGS__), DETAIL_ZELDA_TRAIT_NAME(__VA_ARGS__))

#define ZELDA_HAS_TYPE(...) \
DETAIL_ZELDA_HAS_TYPE(PP_HEAD(__VA_ARGS__), DETAIL_ZELDA_TRAIT_NAME(__VA_ARGS__))

#define ZELDA_HAS_TEMPLATE(...) \
DETAIL_ZELDA_HAS_TEMPLATE(PP_HEAD(__VA_ARGS__), DETAIL_ZELDA_TRAIT_NAME(__VA_ARGS__))

#define DETAIL_ZELDA_TRAIT_NAME_1(x) PP_CAT(has_, x)
#define DETAIL_ZELDA_TRAIT_NAME_2(x, y) y
#define DETAIL_ZELDA_TRAIT_NAME(...) \
PP_CAT(DETAIL_ZELDA_TRAIT_NAME_, PP_NARGS(__VA_ARGS__))(__VA_ARGS__)

#define DETAIL_ZELDA_HAS_MEMBER(member, trait) \
template<class Zelda_I_C, class Zelda_I_T = void> \
struct trait \
{ \
    typedef char yes; \
    typedef long no; \
    template<class Zelda_I_TT, class Zelda_I_Enable = ZELDA_CLASS_REQUIRES(zelda::introspection::check_type<Zelda_I_TT, Zelda_I_T>)> \
    struct selector {}; \
\
    template<class Zelda_I_U> \
    static yes check( selector<ZELDA_XTYPEOF( zelda::declval<Zelda_I_U>().member )> * ); \
\
    template<class Zelda_I_U> \
    static no check(...); \
    \
    static const bool value = sizeof(check<Zelda_I_C>(0)) == sizeof(yes); \
\
    typedef zelda::tpl::bool_<value> type; \
}; \
template<class Zelda_I_C, class Zelda_I_R, class... Zelda_I_Args> \
struct trait<Zelda_I_C, Zelda_I_R(Zelda_I_Args...)> \
{ \
    typedef char yes; \
    typedef long no; \
    template<class Zelda_I_TT, class Zelda_I_Enable = ZELDA_CLASS_REQUIRES(zelda::introspection::check_type<Zelda_I_TT, Zelda_I_R>)> \
    struct selector {}; \
\
    template<class Zelda_I_U> \
    static yes check( selector<ZELDA_XTYPEOF( zelda::declval<Zelda_I_U>().member(zelda::declval<Zelda_I_Args>()...) )> * ); \
\
    template<class Zelda_I_U> \
    static no check(...); \
    \
    static const bool value = sizeof(check<Zelda_I_C>(0)) == sizeof(yes); \
\
    typedef zelda::tpl::bool_<value> type; \
}; \

#define DETAIL_ZELDA_HAS_STATIC_MEMBER(member, trait) \
template<class Zelda_I_C, class Zelda_I_T = void> \
struct trait \
{ \
    typedef char yes; \
    typedef long no; \
    template<class Zelda_I_TT, class Zelda_I_Enable = ZELDA_CLASS_REQUIRES(zelda::introspection::check_type<Zelda_I_TT, Zelda_I_T>)> \
    struct selector {}; \
\
    template<class Zelda_I_U> \
    static yes check( selector<ZELDA_XTYPEOF( Zelda_I_U::member )> * ); \
\
    template<class Zelda_I_U> \
    static no check(...); \
    \
    static const bool value = sizeof(check<Zelda_I_C>(0)) == sizeof(yes); \
\
    typedef zelda::tpl::bool_<value> type; \
}; \
template<class Zelda_I_C, class Zelda_I_R, class... Zelda_I_Args> \
struct trait<Zelda_I_C, Zelda_I_R(Zelda_I_Args...)> \
{ \
    typedef char yes; \
    typedef long no; \
    template<class Zelda_I_TT, class Zelda_I_Enable = ZELDA_CLASS_REQUIRES(zelda::introspection::check_type<Zelda_I_TT, Zelda_I_R>)> \
    struct selector {}; \
\
    template<class Zelda_I_U> \
    static yes check( selector<ZELDA_XTYPEOF( Zelda_I_U::member(zelda::declval<Zelda_I_Args>()...) )> * ); \
\
    template<class Zelda_I_U> \
    static no check(...); \
    \
    static const bool value = sizeof(check<Zelda_I_C>(0)) == sizeof(yes); \
\
    typedef zelda::tpl::bool_<value> type; \
};


#define DETAIL_ZELDA_HAS_TYPE(type,trait) \
  template<class Zelda_I_C, class Zelda_I_T = zelda::introspection::void_> \
  struct trait \
    { \
    typedef char yes; \
    typedef long no; \
    template<class Zelda_I_TT, class Zelda_I_Enable = ZELDA_CLASS_REQUIRES(zelda::introspection::check_type<Zelda_I_TT, Zelda_I_T>)> \
    struct selector {}; \
    \
    template<class Zelda_I_U> \
    static yes check(selector<typename Zelda_I_U::type> *); \
    \
    template<class Zelda_I_U> \
    static no check(...); \
    \
    static const bool value=sizeof(check<Zelda_I_C>(0))==sizeof(yes); \
    \
    typedef zelda::tpl::bool_<value> type; \
    }; \

#define DETAIL_ZELDA_HAS_TEMPLATE(tpl_name, trait) \
  template<class Zelda_I_C> \
  struct trait \
    { \
    typedef char yes; \
    typedef long no; \
    template<template<class...> class Zelda_I_TT> \
    struct selector {}; \
    \
    template<class Zelda_I_U> \
    static yes check(selector<Zelda_I_U::template tpl_name> *); \
    \
    template<class Zelda_I_U> \
    static no check(...); \
    \
    static const bool value=sizeof(check<Zelda_I_C>(0))==sizeof(yes); \
    \
    typedef zelda::tpl::bool_<value> type; \
    }; \


#ifdef ZELDA_TEST

static_assert(zelda::introspection::check_type<int, void>::value, "Check type failed for conversion to void");
static_assert(zelda::introspection::check_type<int, zelda::introspection::void_>::value, "Check type failed for conversion to void_");
static_assert(zelda::introspection::check_type<int, double>::value, "Check type failed for conversion to double");

namespace zelda { namespace introspection_test{

struct introspect_type {};

struct introspection_pass_t
{
    typedef introspect_type type;
    template<class T>
    struct tpl_xxx
    {
        /* data */
    };

    static introspect_type static_method();
    introspect_type method();
    int var;
    static const introspect_type static_var;
};

struct introspection_mismatch_t
{
    typedef int type;
    template<class T>
    struct tpl_xxx
    {
        /* data */
    };

    static int static_method();
    int method();
    introspect_type var;
    static const int static_var;
};

struct introspection_missing_t
{
    typedef introspect_type tpl_xxx;
    template<class T>
    struct type
    {
    };
    void method(int, int);
};

ZELDA_HAS_TYPE(type);
ZELDA_HAS_TEMPLATE(tpl_xxx);
ZELDA_HAS_MEMBER(method);
ZELDA_HAS_MEMBER(var);
ZELDA_HAS_STATIC_MEMBER(static_method);
ZELDA_HAS_STATIC_MEMBER(static_var);


static_assert(has_type<introspection_pass_t, introspect_type>::value, "Type failed");
static_assert(has_tpl_xxx<introspection_pass_t>::value, "Template failed");
static_assert(has_method<introspection_pass_t, introspect_type()>::value, "Method failed");
static_assert(has_var<introspection_pass_t, int>::value, "Var failed");
static_assert(has_static_method<introspection_pass_t, introspect_type()>::value, "Static Method failed");
static_assert(has_static_var<introspection_pass_t, introspect_type>::value, "Static Var failed");

static_assert(has_var<introspection_pass_t>::value, "Simple Var failed");
static_assert(has_static_var<introspection_pass_t>::value, "Simple Static Var failed");

static_assert(not has_type<introspection_mismatch_t, introspect_type>::value, "Type failed");
static_assert(not has_method<introspection_mismatch_t, introspect_type()>::value, "Method failed");
static_assert(not has_var<introspection_mismatch_t, int>::value, "Var failed");
static_assert(not has_static_method<introspection_mismatch_t, introspect_type()>::value, "Static Method failed");
static_assert(not has_static_var<introspection_mismatch_t, introspect_type>::value, "Static Var failed");

static_assert(not has_type<introspection_missing_t, introspect_type>::value, "Type failed");
static_assert(not has_tpl_xxx<introspection_missing_t>::value, "Template failed");
static_assert(not has_method<introspection_missing_t, introspect_type()>::value, "Method failed");
static_assert(not has_var<introspection_missing_t, int>::value, "Var failed");
static_assert(not has_static_method<introspection_missing_t, introspect_type()>::value, "Static Method failed");
static_assert(not has_static_var<introspection_missing_t, introspect_type>::value, "Static Var failed");

}}
#endif


#endif	/* ZELDA_INTROSPECTION_H */

