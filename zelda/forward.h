/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    forward.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FORWARD_H
#define	FORWARD_H

#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/result_of.hpp>
#include <utility>
#include <zelda/pp.h>
#include <zelda/config.h>
#include <zelda/result_of.h> 
#include <zelda/adaptor.h> 


#define ZELDA_FORWARD_MAKE_CONST_SEQ(n) \
BOOST_PP_SEQ_FOR_EACH_PRODUCT(ZELDA_DETAIL_FORWARD_MAKE_CONST_SEQ_PRODUCT, ZELDA_PP_GEN(n, ((ZELDA_PP_EMPTY)(const ZELDA_PP_EMPTY)) BOOST_PP_INTERCEPT))
#define ZELDA_FORWARD_MAKE_CONST_SEQ_ZR(z, r, n) \
BOOST_PP_SEQ_FOR_EACH_PRODUCT(ZELDA_DETAIL_FORWARD_MAKE_CONST_SEQ_PRODUCT, ZELDA_PP_GEN_Z(z, n, ((ZELDA_PP_EMPTY)(const ZELDA_PP_EMPTY)) BOOST_PP_INTERCEPT))
#define ZELDA_DETAIL_FORWARD_MAKE_CONST_SEQ_PRODUCT(r, seq) (seq)

#define ZELDA_FORWARD_PARAMS(const_seq, ...) BOOST_PP_SEQ_ENUM(ZELDA_PP_SEQ_TRANSFORM_I(ZELDA_DETAIL_FORWARD_PARAMS_EACH, (__VA_ARGS__), const_seq))
#define ZELDA_FORWARD_PARAMS_D(d, const_seq, ...) BOOST_PP_SEQ_ENUM(ZELDA_PP_SEQ_TRANSFORM_I_D(d, ZELDA_DETAIL_FORWARD_PARAMS_EACH, (__VA_ARGS__), const_seq))
#define ZELDA_DETAIL_FORWARD_PARAMS_EACH(r, data, i, const_) const_() ZELDA_PP_VARN_CAT(i, data)
// #define ZELDA_DETAIL_FORWARD_PARAMS_INVOKE(data) ZELDA_DETAIL_FORWARD_PARAMS_OP data
// #define ZELDA_DETAIL_FORWARD_PARAMS_OP(const_, i, data) const_() ZELDA_PP_VARN_CAT(i, data)
// #define ZELDA_DETAIL_FORWARD_PARAMS_REF(x) BOOST_PP_IIF(ZELDA_PP_IS_EMPTY(x),,& x)

#define ZELDA_FORWARD_ARGS(const_seq, type, var) BOOST_PP_SEQ_ENUM(ZELDA_PP_SEQ_TRANSFORM_I(ZELDA_DETAIL_FORWARD_ARGS_EACH, (type, var), const_seq))
#define ZELDA_FORWARD_ARGS_D(d, const_seq, type, var) BOOST_PP_SEQ_ENUM(ZELDA_PP_SEQ_TRANSFORM_I_D(d, ZELDA_DETAIL_FORWARD_ARGS_EACH, (type, var), const_seq))
#define ZELDA_DETAIL_FORWARD_ARGS_EACH(r, data, i, const_) ZELDA_DETAIL_FORWARD_ARGS_INVOKE((const_, i, ZELDA_PP_REM data))
#define ZELDA_DETAIL_FORWARD_ARGS_INVOKE(data) ZELDA_DETAIL_FORWARD_ARGS_OP data
#define ZELDA_DETAIL_FORWARD_ARGS_OP(const_, i, T, x) zelda::forward<const_() T ## i>(x ## i)


#define ZELDA_FORWARD_REPEAT(n, macro, data) BOOST_PP_REPEAT(n, ZELDA_DETAIL_FORWARD_REPEAT_X, (macro, data)) 
#define ZELDA_DETAIL_FORWARD_REPEAT_X(z, n, data) \
BOOST_PP_SEQ_FOR_EACH_I(ZELDA_DETAIL_FORWARD_REPEAT_EACH, (z, BOOST_PP_INC(n), ZELDA_PP_REM data), ZELDA_FORWARD_MAKE_CONST_SEQ(BOOST_PP_INC(n)))

#define ZELDA_FORWARD_REPEAT_ZR(z, r, n, macro, data) BOOST_PP_REPEAT_ ## z(n, ZELDA_DETAIL_FORWARD_REPEAT_XR, (r)(macro, data))
#define ZELDA_DETAIL_FORWARD_REPEAT_XR(z, n, data) ZELDA_DETAIL_FORWARD_REPEAT_XR_I(z, BOOST_PP_SEQ_HEAD(data), n, ZELDA_PP_EAT data)
#define ZELDA_DETAIL_FORWARD_REPEAT_XR_I(z, r, n, data) \
BOOST_PP_SEQ_FOR_EACH_I_R(r, ZELDA_DETAIL_FORWARD_REPEAT_EACH, (z, BOOST_PP_INC(n), ZELDA_PP_REM data), ZELDA_FORWARD_MAKE_CONST_SEQ_ZR(z, r, BOOST_PP_INC(n)))

#define ZELDA_DETAIL_FORWARD_REPEAT_EACH(r, data, i, elem) ZELDA_DETAIL_FORWARD_REPEAT_INVOKE((r, i, elem, ZELDA_PP_REM data))
#define ZELDA_DETAIL_FORWARD_REPEAT_INVOKE(data) ZELDA_DETAIL_FORWARD_REPEAT_OP data
#define ZELDA_DETAIL_FORWARD_REPEAT_OP(r, i, elem, z, n, macro, data) macro(z, r, elem, n, data)



#if defined(ZELDA_NO_RVALUE_REFS)
#define ZELDA_FORWARD_CALL_OPERATOR(F, f) \
BOOST_PP_REPEAT(8, ZELDA_DETAIL_FORWARD_CALL_OPERATOR_RESULT, F) \
ZELDA_FORWARD_REPEAT(7, ZELDA_DETAIL_FORWARD_CALL_OPERATOR_OP, (F, f))
#define ZELDA_FORWARD_CALL_OPERATOR_ZR(z, r, F, f) \
BOOST_PP_REPEAT_ ## z(8, ZELDA_DETAIL_FORWARD_CALL_OPERATOR_RESULT, F) \
ZELDA_FORWARD_REPEAT_ZR(z, r, 7, ZELDA_DETAIL_FORWARD_CALL_OPERATOR_OP, (F, f))

#define ZELDA_DETAIL_FORWARD_CALL_OPERATOR_RESULT(z, n, F) \
template<class X BOOST_PP_COMMA_IF(n) ZELDA_PP_PARAMS_Z(z, n, class T)> \
struct result<X(ZELDA_PP_PARAMS_Z(z, n, T))> : zelda::result_of<F(ZELDA_PP_PARAMS_Z(z, n, T))> {}; \

#define ZELDA_DETAIL_FORWARD_CALL_OPERATOR_OP(z, r, cs, n, data) ZELDA_DETAIL_FORWARD_CALL_OPERATOR_INVOKE((z, r, cs, n, ZELDA_PP_REM data))
#define ZELDA_DETAIL_FORWARD_CALL_OPERATOR_INVOKE(data) ZELDA_DETAIL_FORWARD_CALL_OPERATOR data
#define ZELDA_DETAIL_FORWARD_CALL_OPERATOR(z, r, cs, n, F, f) \
template<ZELDA_PP_PARAMS_Z(z, n, class T)> \
typename zelda::result_of<F(ZELDA_FORWARD_PARAMS(cs, T, & BOOST_PP_INTERCEPT))>::type \
operator()(ZELDA_FORWARD_PARAMS(cs, T, &x)) const \
{ \
    return f(ZELDA_FORWARD_ARGS(cs, T, x)); \
}

#elif defined(ZELDA_NO_VARIADIC_TEMPLATES)

#define ZELDA_FORWARD_CALL_OPERATOR(F, f) BOOST_PP_REPEAT(8, ZELDA_DETAIL_FORWARD_CALL_OPERATOR_OP, (F, f))
#define ZELDA_FORWARD_CALL_OPERATOR_ZR(z, r, F, f) BOOST_PP_REPEAT_ ## z(8, ZELDA_DETAIL_FORWARD_CALL_OPERATOR_OP, (F, f))

#define ZELDA_DETAIL_FORWARD_CALL_FORWARD(n) BOOST_PP_ENUM(n, ZELDA_DETAIL_FORWARD_CALL_FORWARD_OP, ~)
#define ZELDA_DETAIL_FORWARD_CALL_FORWARD_Z(z, n) BOOST_PP_ENUM_ ## z(n, ZELDA_DETAIL_FORWARD_CALL_FORWARD_OP, ~)
#define ZELDA_DETAIL_FORWARD_CALL_FORWARD_OP(z, n, data) zelda::forward<T ## n>(x ## n)
#define ZELDA_DETAIL_FORWARD_CALL_OPERATOR_OP(z, n, data) ZELDA_DETAIL_FORWARD_CALL_OPERATOR_INVOKE((z, BOOST_PP_INC(n), ZELDA_PP_REM data))
#define ZELDA_DETAIL_FORWARD_CALL_OPERATOR_INVOKE(data) ZELDA_DETAIL_FORWARD_CALL_OPERATOR data
#define ZELDA_DETAIL_FORWARD_CALL_OPERATOR(z, n, F, f) \
template<class X, ZELDA_PP_PARAMS_Z(z, n, class T)> \
struct result<X(ZELDA_PP_PARAMS_Z(z, n, T))> : zelda::result_of<F(ZELDA_PP_PARAMS_Z(z, n, T))> {}; \
template<ZELDA_PP_PARAMS_Z(z, n, class T)> \
typename zelda::result_of<F(ZELDA_PP_PARAMS_Z(z, n, T, && BOOST_PP_INTERCEPT))>::type \
operator()(ZELDA_PP_PARAMS_Z(z, n, T, && x)) const \
{ \
    return f(ZELDA_DETAIL_FORWARD_CALL_FORWARD_Z(z, n)); \
}

#else

#define ZELDA_FORWARD_CALL_OPERATOR(F, f) ZELDA_DETAIL_FORWARD_CALL_OPERATOR(F, f)
#define ZELDA_FORWARD_CALL_OPERATOR_ZR(z, r, F, f) ZELDA_DETAIL_FORWARD_CALL_OPERATOR(F, f)

#define ZELDA_DETAIL_FORWARD_CALL_OPERATOR(F, f) \
template<class X, class... T> \
struct result<X(T...)> : zelda::result_of<F(T...)> {}; \
template<class... T> \
typename zelda::result_of<F(T && ...)>::type \
operator()(T && ... x) const \
{ \
    return f(zelda::forward<T>(x)...); \
}



#endif


namespace zelda {



#ifdef ZELDA_NO_RVALUE_REFS
#define ZELDA_FORWARD_REF(...) __VA_ARGS__ &
#else
#define ZELDA_FORWARD_REF(...) __VA_ARGS__ &&

template< class T >
ZELDA_FORWARD_REF(T) forward( typename boost::remove_reference<T>::type&& t )
{
    return static_cast<ZELDA_FORWARD_REF(T)>(t);
}

#endif

template< class T >
ZELDA_FORWARD_REF(T) forward( typename boost::remove_reference<T>::type& t )
{
    return static_cast<ZELDA_FORWARD_REF(T)>(t);
}


#define ZELDA_SIMPLE_FORWARD(n, type, var) BOOST_PP_ENUM(n, ZELDA_DETAIL_SIMPLE_FORWARD_EACH, (type, var))
#define ZELDA_SIMPLE_FORWARD_Z(z, n, type, var) BOOST_PP_ENUM_ ## z(n, ZELDA_DETAIL_SIMPLE_FORWARD_EACH, (type, var))
#define ZELDA_DETAIL_SIMPLE_FORWARD_EACH(z, i, data) ZELDA_DETAIL_SIMPLE_FORWARD_INVOKE((i, ZELDA_PP_REM data))
#define ZELDA_DETAIL_SIMPLE_FORWARD_INVOKE(data) ZELDA_DETAIL_SIMPLE_FORWARD_OP data
#define ZELDA_DETAIL_SIMPLE_FORWARD_OP(i, T, x) zelda::forward<T ## i>(x ## i)




template<class F, class Enable = void>
struct forward_adaptor
{
    F f;
    forward_adaptor() {}
    forward_adaptor(F f) : f(f) {}
    template<class X>
    struct result;

    ZELDA_FORWARD_CALL_OPERATOR_ZR(1, 1, F, f)
};

template<class F>
struct forward_adaptor<F, ZELDA_CLASS_REQUIRES(boost::is_empty<F>)>
{
    template<class X>
    struct result;

    ZELDA_FORWARD_CALL_OPERATOR_ZR(1, 1, F, F())
};
}


#define ZELDA_FORWARD_MAKE_ADAPTOR(name, base) \
template<class F, class Enable = void> \
struct name \
{ \
    base<F> f; \
    const base<F>& get_function() const \
    { \
        return f; \
    } \
    template<class X> \
    struct result; \
    ZELDA_FORWARD_CALL_OPERATOR_ZR(1, 1, F, f) \
    \
}; \
template<class F> \
struct name<F, ZELDA_CLASS_REQUIRES(boost::is_empty<base<F> >)>  \
{ \
    base<F> get_function() const \
    { \
        return base<F>(); \
    } \
    template<class X> \
    struct result; \
    ZELDA_FORWARD_CALL_OPERATOR_ZR(1, 1, base<F>, base<F>()) \
}; \



#endif	/* FORWARD_H */

