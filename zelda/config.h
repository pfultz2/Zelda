/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    config.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifdef _MSC_VER

#define ZELDA_NO_RANGE_FOR
#define ZELDA_NO_VARIADIC_TEMPLATES
#define ZELDA_NO_OVERRIDE_CONTROL
#define ZELDA_NO_EXPRESSION_SFINAE


#endif

#ifdef __clang__
#if !__has_feature(cxx_decltype)
#define ZELDA_NO_DECLTYPE
#else
#define ZELDA_HAS_TYPEOF
#endif
#if !__has_feature(cxx_lambdas)
#define ZELDA_NO_LAMBDAS
#endif
#if !__has_feature(cxx_range_for)
#define ZELDA_NO_RANGE_FOR
#endif
#if !__has_feature(cxx_rvalue_references)
#define ZELDA_NO_RVALUE_REFS
#endif
#if !__has_feature(cxx_static_assert)
#define ZELDA_NO_STATIC_ASSERT
#endif
#if !__has_feature(cxx_auto_type)
#define ZELDA_NO_AUTO
#endif
#if !__has_feature(cxx_trailing_return)
#define ZELDA_NO_TRAILING_RETURN
#endif
#if !__has_feature(cxx_variadic_templates)
#define ZELDA_NO_VARIADIC_TEMPLATES
#endif

#if !__has_feature(cxx_override_control)
#define ZELDA_NO_OVERRIDE_CONTROL
#endif

#endif

#ifdef __GNUC__
#define ZELDA_HAS_TYPEOF
#ifndef __clang__
//#define ZELDA_NO_EXPRESSION_SFINAE

#define ZELDA_NO_DECLTYPE
#define ZELDA_NO_LAMBDAS
#define ZELDA_NO_RANGE_FOR
#define ZELDA_NO_RVALUE_REFS
#define ZELDA_NO_STATIC_ASSERT
#define ZELDA_NO_AUTO
#define ZELDA_NO_TRAILING_RETURN
#define ZELDA_NO_VARIADIC_TEMPLATES
#define ZELDA_NO_OVERRIDE_CONTROL
#endif


#ifndef ZELDA_PARAMS_LIMIT
#define ZELDA_PARAMS_LIMIT 6
#endif


#endif