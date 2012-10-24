/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    pipable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_PIPABLE_H
#define ZELDA_GUARD_FUNCTION_PIPABLE_H

#include <zelda/function/adaptor.h>
#include <zelda/function/poly.h>
#include <zelda/forward.h>

namespace zelda { 

namespace detail {

template<class F, class Sequence = zelda::tuple<>, class Enable = void>
struct pipe_closure_base : F
{
    F f;
    Sequence seq;
    
    template<class X, class S>
    pipe_closure_base(ZELDA_FORWARD_REF(S) seq) : seq(zelda::forward<S>(seq)) {};
    
    template<class X, class S>
    pipe_closure_base(X f, ZELDA_FORWARD_REF(S) seq) : F(f), seq(zelda::forward<S>(seq)) {};

    Sequence get_sequence() const
    {
        return seq;
    }

};

template<class F, class Sequence>
struct pipe_closure_base<F, Sequence, ZELDA_CLASS_REQUIRES(boost::is_empty<F>, boost::mpl::bool_<zelda::tuple_size<Sequence>::value == 0>)> : F
{    
    template<class S>
    pipe_closure_base(ZELDA_FORWARD_REF(S)) {};
    
    template<class X, class S>
    pipe_closure_base(X, ZELDA_FORWARD_REF(S)) {};

    Sequence get_sequence() const
    {
        return Sequence();
    }

};

template<class F, class Sequence>
struct pipe_closure : pipe_closure_base<F, Sequence>
{
    
    template<class S>
    pipe_closure(ZELDA_FORWARD_REF(S) seq) : pipe_closure_base<F, Sequence>(zelda::forward<S>(seq)) {};
    
    template<class S>
    pipe_closure(F f, ZELDA_FORWARD_REF(S) seq) : pipe_closure_base<F, Sequence>(f, zelda::forward<S>(seq)) {};

    template<class A>
    struct pipe_result
    : zelda::invoke_result<F, zelda::tuple_cat_result
        <
            zelda::tuple<A>,
            Sequence
        > >
    {};
#ifndef ZELDA_NO_RVALUE_REFS
    template<class A>
    friend typename pipe_result<A&&>::type 
    operator|(A && a, const pipe_closure<F, Sequence>& p)
    {
        return zelda::invoke(p, zelda::tuple_cat
        (
            zelda::forward_as_tuple(zelda::forward<A>(a)), 
            zelda::forward<Sequence>(p.get_sequence())
        ));
    }
#else
    template<class A>
    friend typename pipe_result<A&>::type 
    operator|(A & a, const pipe_closure<F, Sequence>& p)
    {
        return zelda::invoke(p, zelda::tuple_cat
        (
            zelda::forward_as_tuple(zelda::forward<A>(a)), 
            zelda::forward<Sequence>(p.get_sequence())
        ));
    }

    template<class A>
    friend typename pipe_result<const A&>::type 
    operator|(const A & a, const pipe_closure<F, Sequence>& p)
    {
        return zelda::invoke(p, zelda::tuple_cat
        (
            zelda::forward_as_tuple(zelda::forward<A>(a)), 
            zelda::forward<Sequence>(p.get_sequence())
        ));
    }
#endif
};

template<class F>
struct make_pipe_closure : function_adaptor_base<F>
{
    make_pipe_closure()
    {};

    template<class X>
    make_pipe_closure(X x) : function_adaptor_base<F>(x)
    {};

    template<class X>
    struct result;

    template<class X, class T>
    struct result<X(T)>
    {
        typedef pipe_closure<F, T> type;
    };

    template<class T>
    pipe_closure<F, ZELDA_FORWARD_REF(T)> 
    operator()(ZELDA_FORWARD_REF(T) t) const
    {
        return pipe_closure<F, ZELDA_FORWARD_REF(T)>(this->get_function(), zelda::forward<T>(t));
    }
};


    
}

template<class F, class FunctionBase=poly_adaptor<F, variadic_adaptor<detail::make_pipe_closure<F> > > >
struct pipable_adaptor 
: detail::pipe_closure<FunctionBase, zelda::tuple<> >
{
    typedef detail::pipe_closure<FunctionBase, zelda::tuple<> > base;

    
    pipable_adaptor() : base(FunctionBase(), zelda::tuple<>())
    {};

    template<class X>
    pipable_adaptor(X x) : base(x, zelda::tuple<>())
    {};
};


template<class F>
pipable_adaptor<F> pipable(F f)
{
    return pipable_adaptor<F>(f);
}

}

#endif
