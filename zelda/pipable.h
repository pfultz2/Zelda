/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    pipable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <zelda/pp.h>
#include <zelda/forward.h>
#include <zelda/requires.h>
#include <zelda/adaptor.h>

namespace zelda {

namespace details {

struct pipe_na
{};

}


template<class F, ZELDA_PP_PARAMS(8, class T, = details::pipe_na BOOST_PP_INTERCEPT)>
struct pipe_closure {};

#ifdef ZELDA_NO_RVALUE_REFS
#define ZELDA_DETAIL_PIPE_CLOSURE_BUILDER_OPERATOR(z, n) \
template<class A> \
friend typename result_of<F(A&, ZELDA_PP_PARAMS_Z(z, n, T))>::type \
operator|(A & a, const pipe_closure<F, ZELDA_PP_FIXED_PARAMS(n, 8, T, details::pipe_na)>& p) \
{ return p.f(a, ZELDA_PP_PARAMS_Z(z, n, p.x)); } \
\
template<class A> \
friend typename result_of<F(const A&, ZELDA_PP_PARAMS_Z(z, n, T))>::type \
operator|(const A & a, const pipe_closure<F, ZELDA_PP_FIXED_PARAMS(n, 8, T, details::pipe_na)>& p) \
{ return p.f(a, ZELDA_PP_PARAMS_Z(z, n, p.x)); }
#else
#define ZELDA_DETAIL_PIPE_CLOSURE_BUILDER_OPERATOR(z, n) \
template<class A> \
friend typename result_of<F(A&&, ZELDA_PP_PARAMS_Z(z, n, T))>::type \
operator|(A && a, const pipe_closure<F, ZELDA_PP_FIXED_PARAMS(n, 8, T, details::pipe_na)>& p) \
{ return p.f(a, ZELDA_PP_PARAMS_Z(z, n, p.x)); }
#endif

#define ZELDA_DETAIL_PIPE_CLOSURE_CONSTRUCT(n) BOOST_PP_ENUM(n, ZELDA_DETAIL_PIPE_CLOSURE_CONSTRUCT_EACH, data)
#define ZELDA_DETAIL_PIPE_CLOSURE_CONSTRUCT_Z(z, n) BOOST_PP_ENUM_ ## z(n, ZELDA_DETAIL_PIPE_CLOSURE_CONSTRUCT_EACH, data)
#define ZELDA_DETAIL_PIPE_CLOSURE_CONSTRUCT_EACH(z, i, data) x ## i(zelda::forward<T ## i>(x ## i))

#define ZELDA_DETAIL_PIPE_CLOSURE_BUILDER(z, n, data) \
template<class F, ZELDA_PP_PARAMS_Z(z, n, class T)> \
struct pipe_closure<F, ZELDA_PP_FIXED_PARAMS(n, 8, T, details::pipe_na)>\
{ \
    F f; \
    ZELDA_PP_GEN(n, T, x, ; BOOST_PP_INTERCEPT) \
    pipe_closure(F f, ZELDA_PP_PARAMS_Z(z, n, T, x)) \
    : f(f), ZELDA_DETAIL_PIPE_CLOSURE_CONSTRUCT_Z(z, n) {} \
    ZELDA_DETAIL_PIPE_CLOSURE_BUILDER_OPERATOR(z, n) \
};
BOOST_PP_REPEAT_FROM_TO(1, 8, ZELDA_DETAIL_PIPE_CLOSURE_BUILDER, ~)
#undef ZELDA_DETAIL_PIPE_CLOSURE_BUILDER


// TODO: Forward args @ ZELDA_PP_PARAMS_Z(z, n, x)
#define ZELDA_PIPABLE_ADAPTOR_BASE_BUILDER(z, n, data) \
template<class X, ZELDA_PP_PARAMS_Z(z, n, class T)> \
struct result<X(ZELDA_PP_PARAMS_Z(z, n, T)), ZELDA_CLASS_REQUIRES(exclude is_callable<F(ZELDA_PP_PARAMS_Z(z, n, T))>)> \
{ typedef pipe_closure<F, ZELDA_PP_PARAMS_Z(z, n, T)> type; };\
\
template<class X, ZELDA_PP_PARAMS_Z(z, n, class T)> \
struct result<X(ZELDA_PP_PARAMS_Z(z, n, T)), ZELDA_CLASS_REQUIRES(is_callable<F(ZELDA_PP_PARAMS_Z(z, n, T))>)> \
: zelda::result_of<F(ZELDA_PP_PARAMS_Z(z, n, T))> {};\
\
template<ZELDA_PP_PARAMS_Z(z, n, class T)> \
typename zelda::result_of<F(ZELDA_PP_PARAMS_Z(z, n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT))>::type \
operator()(ZELDA_PP_PARAMS_Z(z, n, T, ZELDA_FORWARD_REF() x)) const \
{ return this->get_function()(ZELDA_SIMPLE_FORWARD_Z(z, n, T, x)); } \
\
template<ZELDA_PP_PARAMS_Z(z, n, class T)> \
ZELDA_FUNCTION_REQUIRES(exclude is_callable<F(ZELDA_PP_PARAMS_Z(z, n, T))>) \
(pipe_closure<F, ZELDA_PP_PARAMS_Z(z, n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT)>) \
operator()(ZELDA_PP_PARAMS_Z(z, n, T, ZELDA_FORWARD_REF() x)) const \
{ return pipe_closure<F, ZELDA_PP_PARAMS_Z(z, n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT)>(this->get_function(), ZELDA_SIMPLE_FORWARD_Z(z, n, T, x)); } \


template<class F, class Enable = void>
struct pipable_adaptor_base
{
    F f;
    const F& get_function() const
    {
        return f;
    }
    pipable_adaptor_base() {}
    pipable_adaptor_base(F f) : f(f) {}

    template<class X, class ResultEnable = void>
    struct result;

BOOST_PP_REPEAT_FROM_TO(1, 8, ZELDA_PIPABLE_ADAPTOR_BASE_BUILDER, ~)

};

template<class F>
struct pipable_adaptor_base<F, ZELDA_CLASS_REQUIRES(boost::is_empty<F>)>
{
    F get_function() const
    {
        return F();
    }

    template<class X, class ResultEnable = void>
    struct result;

BOOST_PP_REPEAT_FROM_TO(1, 8, ZELDA_PIPABLE_ADAPTOR_BASE_BUILDER, ~)

};

#undef ZELDA_PIPABLE_ADAPTOR_BASE_BUILDER

ZELDA_FORWARD_MAKE_ADAPTOR(pipable_adaptor, pipable_adaptor_base)

#ifdef ZELDA_NO_RVALUE_REFS
template<class A, class F>
typename result_of<F(A&)>::type
operator|(A & a, pipable_adaptor<F> p)
{
    return p.get_function()(a);
}

template<class A, class F>
typename result_of<F(const A&)>::type
operator|(const A & a, const pipable_adaptor<F>& p)
{
    return p.get_function()(a);
}
#else

template<class A, class F>
typename result_of<F(A&&)>::type
operator|(A && a, const pipable_adaptor<F>& p)
{
    return p.get_function()(zelda::forward<A>(a));
}

#endif


}