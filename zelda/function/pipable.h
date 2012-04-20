/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    pipable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_PIPABLE_H
#define ZELDA_GUARD_FUNCTION_PIPABLE_H

#include <zelda/function/adaptor.h>

namespace zelda { 

//Pipable
namespace details {
template<class F, class Sequence>
struct pipe_closure
{
    F f;
    const Sequence & seq;
    

    pipe_closure(const Sequence & seq) : seq(seq) {};
    

    pipe_closure(F f, const Sequence & seq) : f(f), seq(seq) {};

    template<class A>
    static auto push_front(A && a, Sequence && seq)
    ZELDA_RETURNS(zelda::tuple_cat(zelda::tuple<A>(a), seq));

    template<class A>
    friend ZELDA_FUNCTION_REQUIRES(zelda::is_callable_tuple<F, typename zelda::tuple_join<tuple<A&&>, Sequence>::type>)
    (typename zelda::result_of_tuple<F, typename zelda::tuple_join<tuple<A&&>, Sequence>::type>::type) 
    operator|(A && a, const pipe_closure<F, Sequence>& p)
    
    {
        return zelda::invoke(p.f, zelda::tuple_cat(zelda::forward_as_tuple(std::forward<A>(a)), const_cast<Sequence&&>(p.seq)));
    }
};

// template<class F, class Seq>
// auto make_pipe_closure(F f, Seq && seq) ZELDA_RETURNS
// ( pipe_closure<F, decltype(zelda::unforward_tuple(seq))>(f, std::forward<Seq>(seq)) )

template<class F, class Seq>
auto make_pipe_closure(F f, const Seq & seq) ZELDA_RETURNS
( pipe_closure<F, Seq>(f, seq) )

}

struct pipable_adaptor_base
{
    ZELDA_AUTO_DECLARE_RESULT()

    ZELDA_AUTO_RESULT( (self, f, seq)(is_callable_tuple<f, seq>) (zelda::invoke(f, seq)) )
    //ZELDA_AUTO_RESULT( (f, seq)(is_callable_seq<f, seq>) (static_assert(false, "is_callable")) )

    ZELDA_AUTO_RESULT( (self, f, seq)(not is_callable_tuple<f, seq>) (details::make_pipe_closure(f, seq)) )
};

template<class F>
struct pipable_adaptor : function_variadic_adaptor<pipable_adaptor<F>, pipable_adaptor_base, F> 
{ 
    typedef function_variadic_adaptor<pipable_adaptor<F>, pipable_adaptor_base, F> base;
    pipable_adaptor() {}; 
    pipable_adaptor(F f) : base(f) {}; 

    template<class A, ZELDA_REQUIRES(zelda::is_callable<F(A)>)>
    friend typename zelda::result_of<F(A)>::type operator|(A && a, const pipable_adaptor<F>& p)
    {
        return p.get_function()(std::forward<A>(a));
    }
}; 
template<class F> 
pipable_adaptor<F> pipable(F f) { return pipable_adaptor<F>(f); }

}

#endif
