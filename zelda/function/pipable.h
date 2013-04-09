/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    pipable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_PIPABLE_H
#define ZELDA_GUARD_FUNCTION_PIPABLE_H

#include <zelda/function/adaptor.h>
#include <zelda/function/conditional.h>
#include <zelda/function/static.h>
#include <zelda/forward.h>
#include <zelda/function/invoke.h>
#include <zelda/function/detail/nullary_tr1_result_of.h>

#include <boost/fusion/include/join.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/type_traits.hpp>

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
#include <tuple>
#include <boost/fusion/adapted/std_tuple.hpp>
#define ZELDA_PIPABLE_SEQUENCE std::tuple
#else
#include <boost/fusion/container/vector.hpp>
#define ZELDA_PIPABLE_SEQUENCE boost::fusion::vector
#endif
namespace zelda { 
 

namespace detail {

template<class F, class Sequence = ZELDA_PIPABLE_SEQUENCE<>, class Enable = void>
struct pipe_closure_base : F
{
    typedef typename boost::remove_cv<typename boost::decay<Sequence>::type>::type sequence; 
    sequence seq;
    
	// We need the static_cast to the base class, 
	// because MSVC generates an incorrect copy constructor
	pipe_closure_base(const pipe_closure_base& rhs) : F(static_cast<const F&>(rhs)), seq(rhs.seq) {}

    template<class X, class S>
    pipe_closure_base(X f, const S& seq) : F(f), seq(seq) {};

    const sequence& get_sequence() const
    {
        return seq;
    }

};

template<class F, class Sequence>
struct pipe_closure_base<F, Sequence, ZELDA_CLASS_REQUIRES(boost::mpl::bool_<boost::fusion::result_of::size<Sequence>::value == 0>)> : F
{    
    
    template<class X, class S>
    pipe_closure_base(X x, S) : F(x) {};

    Sequence get_sequence() const
    {
        return Sequence();
    }

};

template<class F, class Sequence>
struct pipe_closure : pipe_closure_base<F, Sequence>
{
    
    template<class X, class S>
    pipe_closure(X f, const S& seq) : pipe_closure_base<F, Sequence>(f, seq) {};

    template<class A>
    struct pipe_result
    : zelda::invoke_result<F, typename boost::fusion::result_of::as_vector<typename boost::fusion::result_of::join
        <
            ZELDA_PIPABLE_SEQUENCE<typename tuple_reference<A>::type>,
            typename boost::decay<Sequence>::type
        >::type>::type >
    {};


// TODO: Use rvalue references when boost::fusion can handle them
// TODO: Update invoke to work on Forward Sequences
#define ZELDA_PIPE_CLOSURE_OP(T) \
    template<class A> \
    friend typename pipe_result<T>::type \
    operator|(ZELDA_FORWARD_REF(T) a, const pipe_closure<F, Sequence>& p) \
    { \
        return zelda::invoke(p, boost::fusion::as_vector(boost::fusion::join \
        ( \
            ZELDA_PIPABLE_SEQUENCE<typename tuple_reference<ZELDA_FORWARD_REF(T)>::type>(zelda::forward<T>(a)), \
            p.get_sequence() \
        ))); \
    }
    ZELDA_PIPE_CLOSURE_OP(A)
#ifdef ZELDA_NO_RVALUE_REFS
    ZELDA_PIPE_CLOSURE_OP(const A)
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
        typedef pipe_closure<F, typename boost::remove_cv<typename boost::decay<T>::type>::type> type;
    };

    template<class T>
    pipe_closure<F, T> 
    operator()(const T& t) const
    {
        return pipe_closure<F, T>(this->get_function(), t);
    }
};
    
}

template<class F, class FunctionBase=conditional_adaptor<F, variadic_adaptor<detail::make_pipe_closure<F> > > >
struct pipable_adaptor 
: detail::pipe_closure<FunctionBase, ZELDA_PIPABLE_SEQUENCE<> >
{
    typedef detail::pipe_closure<FunctionBase, ZELDA_PIPABLE_SEQUENCE<> > base;

    
    pipable_adaptor() : base(FunctionBase(), ZELDA_PIPABLE_SEQUENCE<>())
    {};

    template<class X>
    pipable_adaptor(X x) : base(x, ZELDA_PIPABLE_SEQUENCE<>())
    {};
};


template<class F>
pipable_adaptor<F> pipable(F f)
{
    return pipable_adaptor<F>(f);
}

// Operators for static_ adaptor
#define ZELDA_PIPE_STATIC_OP(T) \
template<class A, class F> \
typename F::template pipe_result<T>::type \
operator|(ZELDA_FORWARD_REF(T) a, static_<F>) \
{ \
    return a | F(); \
}
ZELDA_PIPE_STATIC_OP(A)
#ifdef ZELDA_NO_RVALUE_REFS
ZELDA_PIPE_STATIC_OP(const A)
#endif
}

ZELDA_NULLARY_TR1_RESULT_OF_N(2, zelda::pipable_adaptor)

#ifdef ZELDA_TEST
#include <zelda/test.h>
#include <zelda/function/detail/test.h>
#include <zelda/function/static.h>

zelda::static_<zelda::pipable_adaptor<binary_class> > binary_pipable = {};

zelda::static_<zelda::pipable_adaptor<unary_class> > unary_pipable = {};

zelda::static_<zelda::pipable_adaptor<mutable_class> > mutable_pipable = {};

zelda::static_<zelda::pipable_adaptor<void_class> > void_pipable = {};

zelda::static_<zelda::pipable_adaptor<mono_class> > mono_pipable = {};

ZELDA_TEST_CASE(pipable_test)
{
    void_pipable(1);
    1 | void_pipable;
    ZELDA_TEST_EQUAL(3, 1 | binary_pipable(2));
    ZELDA_TEST_EQUAL(3, binary_pipable(1, 2));
    ZELDA_TEST_EQUAL(3, 3 | unary_pipable);
    ZELDA_TEST_EQUAL(3, unary_pipable(3));
    int i1 = 1;
    ZELDA_TEST_EQUAL(3, 2 | binary_pipable(i1));
    ZELDA_TEST_EQUAL(3, i1 | mutable_pipable(2));
    int i2 = 1;
    ZELDA_TEST_EQUAL(3, mutable_pipable(i2, 2));
    ZELDA_TEST_EQUAL(3, mono_pipable(2));
    ZELDA_TEST_EQUAL(3, 2| mono_pipable);
}
#endif

#endif
