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
    typedef const typename boost::remove_cv<typename boost::decay<Sequence>::type>::type& sequence; 
    F f;
    sequence seq;
    
    template<class S>
    pipe_closure_base(const S& seq) : seq(seq) {};
    
    template<class X, class S>
    pipe_closure_base(X f, const S& seq) : F(f), seq(seq) {};

    sequence get_sequence() const
    {
        return seq;
    }

};

template<class F, class Sequence>
struct pipe_closure_base<F, Sequence, ZELDA_CLASS_REQUIRES(boost::is_empty<F>, boost::mpl::bool_<zelda::tuple_size<Sequence>::value == 0>)> : F
{    
    template<class S>
    pipe_closure_base(S) {};
    
    template<class X, class S>
    pipe_closure_base(X, S) {};

    Sequence get_sequence() const
    {
        return Sequence();
    }

};

template<class F, class Sequence>
struct pipe_closure : pipe_closure_base<F, Sequence>
{
    
    // template<class X, class S>
    // pipe_closure(const pipe_closure<X, S>& rhs) : pipe_closure_base<F, Sequence>(rhs, rhs.get_sequence())
    // {}

    template<class S>
    pipe_closure(const S& seq) : pipe_closure_base<F, Sequence>(seq) {};
    
    template<class X, class S>
    pipe_closure(X f, const S& seq) : pipe_closure_base<F, Sequence>(f, seq) {};

    template<class A>
    struct pipe_result
    : zelda::invoke_result<F, typename zelda::tuple_cat_result
        <
            zelda::tuple<typename add_tuple_forward_reference<A>::type>,
            Sequence
        >::type >
    {};
#ifndef ZELDA_NO_RVALUE_REFS
    template<class A>
    friend typename pipe_result<A&&>::type 
    operator|(A && a, const pipe_closure<F, Sequence>& p)
    {
        return zelda::invoke(p, zelda::tuple_cat
        (
            zelda::forward_as_tuple(zelda::forward<A>(a)), 
            p.get_sequence()
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
            p.get_sequence()
        ));
    }

    template<class A>
    friend typename pipe_result<const A&>::type 
    operator|(const A & a, const pipe_closure<F, Sequence>& p)
    {
        return zelda::invoke(p, zelda::tuple_cat
        (
            zelda::forward_as_tuple(zelda::forward<const A>(a)), 
            p.get_sequence()
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
        typedef pipe_closure<F, typename boost::decay<T>::type> type;
    };

    template<class T>
    pipe_closure<F, T> 
    operator()(const T& t) const
    {
        return pipe_closure<F, T>(this->get_function(), t);
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
