/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    partial.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_PARTIAL_H
#define ZELDA_GUARD_FUNCTION_PARTIAL_H

#include <zelda/function/adaptor.h>
#include <zelda/function/poly.h>
#include <zelda/function/static.h>
#include <zelda/forward.h>
#include <zelda/function/invoke.h>

#include <boost/fusion/include/join.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/type_traits.hpp>

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
#include <tuple>
#include <boost/fusion/adapted/std_tuple.hpp>
#define ZELDA_PARTIAL_SEQUENCE std::tuple
#else
#include <boost/fusion/container/vector.hpp>
#define ZELDA_PARTIAL_SEQUENCE boost::fusion::vector
#endif


#include <zelda/function/reveal.h>

namespace zelda { 

// Forward declare partial_adaptor, since it will be used below
template<class F, class Sequence=ZELDA_PARTIAL_SEQUENCE<> >
struct partial_adaptor;

template<class F>
partial_adaptor<F> partial(F f)
{
    return partial_adaptor<F>(f);
}

template<class F, class Sequence>
partial_adaptor<F, Sequence> partial(F f, Sequence seq)
{
    return partial_adaptor<F, Sequence>(f, seq);
}

namespace detail {


struct partial_adaptor_invoke
{
    template<class>
    struct result;

    template<class X, class F, class Sequence, class T>
    struct result<X(F, Sequence, T)>
    : zelda::result_of<typename zelda::purify<F>::type(typename boost::fusion::result_of::as_vector<typename boost::fusion::result_of::join
    <
        typename boost::decay<Sequence>::type,
        typename boost::decay<T>::type
    >::type>::type)>
    {};

    template<class F, class Sequence, class T>
    typename result<partial_adaptor_invoke(F, Sequence, T)>::type
    operator()(F f, const Sequence& seq, const T& x) const
    {
        return  f(boost::fusion::as_vector(boost::fusion::join
        (
            seq,
            x
        )));
    }
};



struct partial_adaptor_join
{

    partial_adaptor_join()
    {}

    struct decay_elem_k
    {
        template<class>
        struct result;

        template<class X, class T>
        struct result<X(T)>
        : boost::decay<T>
        {};

        template<class T>
        typename boost::decay<T>::type operator()(ZELDA_FORWARD_REF(T) x) const
        {
            return x;
        }
    };
    typedef perfect_adaptor<decay_elem_k> decay_elem;

    template<class>
    struct result;

    template<class X, class F, class Sequence, class T>
    struct result<X(F, Sequence, T)>
    {
        typedef partial_adaptor
        <
            variadic_adaptor<typename zelda::purify<F>::type>, 
            typename boost::fusion::result_of::as_vector<typename boost::fusion::result_of::transform<typename boost::fusion::result_of::join
            <
                typename boost::decay<Sequence>::type,
                typename boost::decay<T>::type
            >::type, decay_elem>::type>::type
        > type;
    };

    template<class F, class Sequence, class T>
    typename result<partial_adaptor_join(F, Sequence, T)>::type
    operator()(F f, const Sequence& seq, const T& x) const
    {
        return partial
        (
            variadic(f), 
            boost::fusion::as_vector(boost::fusion::transform(boost::fusion::join
            (
                seq,
                x
            ), decay_elem()))
        );
    }
};

typedef zelda::poly_adaptor<partial_adaptor_invoke, partial_adaptor_join > partial_cond;

template<class F, class Sequence, class Enable = void>
struct partial_adaptor_base : partial_cond, zelda::function_adaptor_base<F>
{
    typedef zelda::function_adaptor_base<F> base;

    Sequence seq;

    partial_adaptor_base(Sequence seq) : seq(seq)
    {}

    template<class X>
    partial_adaptor_base(X x, Sequence seq) : base(x), seq(seq)
    {}

    using partial_cond::operator();

    template<class>
    struct result;

    template<class X, class T>
    struct result<X(T)>
    : zelda::result_of<partial_cond(F, Sequence, T)>
    {};

    template<class T>
    typename result<partial_adaptor_base(T)>::type
    operator()(const T& x) const
    {
        return (*this)(this->base::get_function(), seq, x);
    }

    template<class X>
    struct result<X()>
    {
        typedef partial_adaptor_base<F, Sequence> type;
    };

    const partial_adaptor_base<F, Sequence>& operator()() const
    {
        return *this;
    }
};

// Empty sequence optimization
template<class F, class Sequence>
struct partial_adaptor_base<F, Sequence, ZELDA_CLASS_REQUIRES(boost::mpl::bool_<boost::fusion::result_of::size<Sequence>::value == 0>)>
: partial_cond, zelda::function_adaptor_base<F>
{

    typedef zelda::function_adaptor_base<F> base;

    partial_adaptor_base()
    {}

    partial_adaptor_base(Sequence)
    {}

    template<class X>
    partial_adaptor_base(X x, Sequence) : base(x)
    {}

    using partial_cond::operator();

    template<class>
    struct result;

    template<class X, class T>
    struct result<X(T)>
    : zelda::result_of<partial_cond(F, Sequence, T)>
    {};

    template<class T>
    typename result<partial_adaptor_base(T)>::type
    operator()(const T& x) const
    {
        return (*this)(this->base::get_function(), Sequence(), x);
    }

    template<class X>
    struct result<X()>
    {
        typedef partial_adaptor_base<F, Sequence> type;
    };

    const partial_adaptor_base<F, Sequence>& operator()() const
    {
        return *this;
    }
};

}

#define ZELDA_PARTIAL_ADAPTOR_BASE(F, Sequence) detail::partial_adaptor_base<F, Sequence >

template<class F, class Sequence >
struct partial_adaptor 
: zelda::variadic_adaptor<ZELDA_PARTIAL_ADAPTOR_BASE(zelda::fuse_adaptor<F>, Sequence) >
{
    typedef ZELDA_PARTIAL_ADAPTOR_BASE(zelda::fuse_adaptor<F>, Sequence) base;
    template<class X>
    partial_adaptor(X x, Sequence seq) : zelda::variadic_adaptor<base>(base(x, seq))
    {}
};

template<class F>
struct partial_adaptor<F, ZELDA_PARTIAL_SEQUENCE<> > 
: zelda::variadic_adaptor<ZELDA_PARTIAL_ADAPTOR_BASE(zelda::fuse_adaptor<F>, ZELDA_PARTIAL_SEQUENCE<>) >
{
    typedef ZELDA_PARTIAL_ADAPTOR_BASE(zelda::fuse_adaptor<F>, ZELDA_PARTIAL_SEQUENCE<>) base;

    partial_adaptor()
    {}

    template<class X>
    partial_adaptor(X x) : zelda::variadic_adaptor<base>(base(x))
    {}
};



// Optimizations for fuse adaptors
template<class F, class Sequence >
struct partial_adaptor<zelda::fuse_adaptor<F>, Sequence>
: ZELDA_PARTIAL_ADAPTOR_BASE(zelda::fuse_adaptor<F>, Sequence)
{
    typedef ZELDA_PARTIAL_ADAPTOR_BASE(zelda::fuse_adaptor<F>, Sequence) base;
    template<class X>
    partial_adaptor(X x, Sequence seq) : base(x, seq)
    {}
};

template<class F>
struct partial_adaptor<zelda::fuse_adaptor<F>, ZELDA_PARTIAL_SEQUENCE<> > 
: ZELDA_PARTIAL_ADAPTOR_BASE(F, ZELDA_PARTIAL_SEQUENCE<>)
{
    typedef ZELDA_PARTIAL_ADAPTOR_BASE(zelda::fuse_adaptor<F>, ZELDA_PARTIAL_SEQUENCE<>) base;

    partial_adaptor()
    {}

    template<class X>
    partial_adaptor(X x) : base(x)
    {}
};

}

#ifdef ZELDA_TEST
#include <zelda/test.h>
#include <zelda/function/detail/test.h>
#include <zelda/function/static.h>

zelda::static_<zelda::partial_adaptor<binary_class> > binary_partial = {};

zelda::static_<zelda::partial_adaptor<unary_class> > unary_partial = {};

zelda::static_<zelda::partial_adaptor<void_class> > void_partial = {};

zelda::static_<zelda::partial_adaptor<mono_class> > mono_partial = {};

ZELDA_TEST_CASE(partial_test)
{
    zelda::partial_adaptor<void_class>()(1);

    void_partial(1);
    // void_partial()(1);
    ZELDA_TEST_EQUAL(3, binary_partial(1)(2));
    ZELDA_TEST_EQUAL(3, binary_partial(1, 2));
    // ZELDA_TEST_EQUAL(3, unary_partial()(3));
    ZELDA_TEST_EQUAL(3, unary_partial(3));
    ZELDA_TEST_EQUAL(3, mono_partial(2));
    // ZELDA_TEST_EQUAL(3, mono_partial()(2));
}
#endif

#endif
