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

#define ZELDA_DETAIL_PIPE_CLOSURE_BUILDER(z, n, data) \
template<class F, ZELDA_PP_PARAMS(n, class T)> \
struct pipe_closure<F, ZELDA_PP_FIXED_PARAMS(n, 8, T, details::pipe_na)>\
{ \
    F f; \
    ZELDA_PP_GEN(n, T, x, ; BOOST_PP_INTERCEPT) \
    pipe_closure(F f, ZELDA_PP_PARAMS(n, T, x)) \
    : f(f), ZELDA_PP_CONSTRUCT(n, x) {} \
    template<class A> \
    friend typename result_of<F(A&, ZELDA_PP_PARAMS(n, T))>::type \
    operator|(A & a, pipe_closure<F, ZELDA_PP_FIXED_PARAMS(n, 8, T, details::pipe_na)> p) \
    { return p.f(a, ZELDA_PP_PARAMS(n, p.x)); } \
    template<class A> \
    friend typename result_of<F(const A&, ZELDA_PP_PARAMS(n, T))>::type \
    operator|(const A & a, pipe_closure<F, ZELDA_PP_FIXED_PARAMS(n, 8, T, details::pipe_na)> p) \
    { return p.f(a, ZELDA_PP_PARAMS(n, p.x)); } \
};
BOOST_PP_REPEAT_FROM_TO(1, 8, ZELDA_DETAIL_PIPE_CLOSURE_BUILDER, ~)
#undef ZELDA_DETAIL_PIPE_CLOSURE_BUILDER

// template<class F, class T0 > struct pipe_closure<F, T0 , details::pipe_na , details::pipe_na , details::pipe_na , details::pipe_na , details::pipe_na , details::pipe_na , details::pipe_na >
// {
//     F f;
//     T0 x0 ;
//     pipe_closure(F f, T0 x0 ) : f(f), x0(x0) {} 
//     template<class A> 
//     friend typename result_of<F(A&, T0 )>::type 
//     operator|(A & a, pipe_closure<F, T0 , details::pipe_na , details::pipe_na , details::pipe_na , details::pipe_na , details::pipe_na , details::pipe_na , details::pipe_na > p)
//     {
//         return p.f(a, p.x0 );
//     } 
//     template<class A> 
//     friend typename result_of<F(const A&, T0 )>::type 
//     operator|(const A & a, pipe_closure<F, T0 , details::pipe_na , details::pipe_na , details::pipe_na , details::pipe_na , details::pipe_na , details::pipe_na , details::pipe_na > p)
//     {
//         return p.f(a, p.x0 );
//     }
// };


template<class F>
struct pipable_adaptor_base : function_base<F>
{
    pipable_adaptor_base() {}
    pipable_adaptor_base(F f) : function_base<F>(f) {}

    template<class X, class Enable = void>
    struct result
    : zelda::result_of<X> {};
// TODO: Forward args @ ZELDA_PP_PARAMS(n, x)
#define ZELDA_PIPABLE_ADAPTOR_BASE_BUILDER(z, n, data) \
template<class X, ZELDA_PP_PARAMS(n, class T)> \
struct result<X(ZELDA_PP_PARAMS(n, T)), ZELDA_CLASS_REQUIRES(exclude is_callable<F(ZELDA_PP_PARAMS(n, T))>)> \
{ typedef pipe_closure<F, ZELDA_PP_PARAMS(n, T)> type; };\
\
template<ZELDA_PP_PARAMS(n, class T)> \
typename zelda::result_of<F(ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT))>::type \
operator()(ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() x)) \
{ return this->get_function()(ZELDA_PP_PARAMS(n, x)); } \
\
template<ZELDA_PP_PARAMS(n, class T)> \
ZELDA_FUNCTION_REQUIRES(exclude is_callable<F(ZELDA_PP_PARAMS(n, T))>) \
(pipe_closure<F, ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT)>) \
operator()(ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() x)) \
{ return pipe_closure<F, ZELDA_PP_PARAMS(n, T, ZELDA_FORWARD_REF() BOOST_PP_INTERCEPT)>(this->get_function(), ZELDA_PP_PARAMS(n, x)); } \

BOOST_PP_REPEAT_FROM_TO(1, 8, ZELDA_PIPABLE_ADAPTOR_BASE_BUILDER, ~)
#undef ZELDA_PIPABLE_ADAPTOR_BASE_BUILDER



// template<class X, class T0 > 
// struct result<X(T0), typename boost::enable_if<zelda::requires_detail::requires_<boost::mpl::vector<zelda::requires_detail::not_tag, is_callable<F( T0 )> > > >::type>
// {
//     typedef pipe_closure<F, T0 > type;
// };
// template< class T0 > 
// typename zelda::result_of<F( T0 & )>::type 
// operator()( T0 & x0 )
// {
//     return this->get_function()( x0 );
// }
// template< class T0 > 
// typename boost::enable_if<zelda::requires_detail::requires_<boost::mpl::vector<zelda::requires_detail::not_tag, is_callable<F( T0 )> > >, pipe_closure<F, T0 & > >::type 
// operator()( T0 & x0 )
// {
//     return pipe_closure<F, T0 & >(this->get_function(), x0 );
// }


};

ZELDA_FORWARD_MAKE_ADAPTOR(pipable_adaptor, pipable_adaptor_base)

template<class A, class F>
typename result_of<F(A&)>::type
operator|(A & a, pipable_adaptor<F> p)
{
    return p.get_function(a);
}

template<class A, class F>
typename result_of<F(const A&)>::type
operator|(const A & a, pipable_adaptor<F> p)
{
    return p.get_function(a);
}


}