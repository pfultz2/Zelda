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
#define ZELDA_DETAIL_FORWARD_PARAMS_EACH(r, data, i, const_) ZELDA_DETAIL_FORWARD_PARAMS_INVOKE((const_, i, ZELDA_PP_REM data, BOOST_PP_INTERCEPT))
#define ZELDA_DETAIL_FORWARD_PARAMS_INVOKE(data) ZELDA_DETAIL_FORWARD_PARAMS_OP data
#define ZELDA_DETAIL_FORWARD_PARAMS_OP(const_, i, T, x, ...) const_() T ## i ZELDA_DETAIL_FORWARD_PARAMS_REF(x ## i)
#define ZELDA_DETAIL_FORWARD_PARAMS_REF(x) BOOST_PP_IIF(ZELDA_PP_IS_EMPTY(x),,& x)

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



template<class F>
struct forward_adaptor : function_base<F>
{
    forward_adaptor() {}
    forward_adaptor(F f) : function_base<F>(f) {}
    template<class X>
    struct result
    : zelda::result_of<X> {};

#if defined(ZELDA_NO_VARIADIC_TEMPLATES) || defined(ZELDA_NO_RVALUE_REFS)

// TODO: Add support for nullary forwarding
// typename zelda::result_of<F()>::type operator()()
// {
//     return this->get_function()();
// }

#define ZELDA_DETAIL_FORWARD_ADAPT(z, r, cs, n, data) \
template<ZELDA_PP_PARAMS(n, class T)> \
typename zelda::result_of<F(ZELDA_FORWARD_PARAMS_D(1, cs, T))>::type \
operator()(ZELDA_FORWARD_PARAMS_D(1, cs, T, x)) const \
{ \
    return this->get_function()(ZELDA_FORWARD_ARGS_D(1, cs, T, x)); \
}
//ZELDA_FORWARD_REPEAT_ZR(1, 1, 2, ZELDA_DETAIL_FORWARD_ADAPT, ~)
#undef ZELDA_DETAIL_FORWARD_ADAPT

template< class T0 > typename zelda::result_of<F(T0)>::type operator()(T0 & x0) const
{
    return this->get_function()(zelda::forward< T0>( x0));
}
template< class T0 > typename zelda::result_of<F(const T0)>::type operator()(const T0 & x0) const
{
    return this->get_function()(zelda::forward<const T0>( x0));
}
template< class T0 , class T1 > typename zelda::result_of<F(T0, T1)>::type operator()(T0 & x0, T1 & x1) const
{
    return this->get_function()(zelda::forward< T0>( x0), zelda::forward< T1>( x1));
}
template< class T0 , class T1 > typename zelda::result_of<F(T0, const T1)>::type operator()(T0 & x0, const T1 & x1) const
{
    return this->get_function()(zelda::forward< T0>( x0), zelda::forward<const T1>( x1));
}
template< class T0 , class T1 > typename zelda::result_of<F(const T0, T1)>::type operator()(const T0 & x0, T1 & x1) const
{
    return this->get_function()(zelda::forward<const T0>( x0), zelda::forward< T1>( x1));
}
template< class T0 , class T1 > typename zelda::result_of<F(const T0, const T1)>::type operator()(const T0 & x0, const T1 & x1) const
{
    return this->get_function()(zelda::forward<const T0>( x0), zelda::forward<const T1>( x1));
}

#else

    template<class... T>
    typename zelda::result_of<F(T...)>::type operator()(T && ... x) const
    {
        return this->get_function()(zelda::forward<T>(x)...);
    }
#endif
};
}


#define ZELDA_FORWARD_MAKE_ADAPTOR(name, base) \
template<class F> \
struct name : zelda::forward_adaptor<base<F> >  \
{ \
    name() {} \
    name(F f) : zelda::forward_adaptor<base<F> >(base<F>(f)) {} \
};



#endif	/* FORWARD_H */

