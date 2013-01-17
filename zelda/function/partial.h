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
#include <boost/type_traits.hpp>

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
#include <tuple>
#include <boost/fusion/adapted/std_tuple.hpp>
#define ZELDA_PARTIAL_SEQUENCE std::tuple
#else
#include <boost/fusion/vector.hpp>
#define ZELDA_PARTIAL_SEQUENCE boost::fusion::vector

namespace zelda { 

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

// template<class F, class Sequence, class Enable = void>
// struct partial_adaptor_base;

// template<class F, class Sequence>
// partial_adaptor_base<F, Sequence> make_partial_adaptor_base(F f, Sequence seq)
// {
//     return partial_adaptor_base<F, Sequence>(f, seq);
// }


struct partial_adaptor_invoke
{
    template<class>
    struct result;

    template<class X, class F, class Sequence, class T>
    struct result<X(Sequence, T)>
    : zelda::invoke_result<F, typename boost::fusion::result_of::as_vector<typename boost::fusion::result_of::join
    <
        typename boost::decay<Sequence>::type,
        typename boost::decay<T>::type
    >::type>::type>
    {};

    template<class F, class Sequence, class T>
    typename result<partial_adaptor_invoke(Sequence, T)>::type
    operator()(const Sequence& seq, const T& x) const
    {
        return zelda::invoke(f, boost::fusion::as_vector(boost::fusion::join
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

    template<class>
    struct result;

    template<class X, class F, class Sequence, class T>
    struct result<X(F, Sequence, T)>
    {
        typedef partial_adaptor<F, typename boost::fusion::result_of::join
        <
            typename boost::decay<Sequence>::type,
            typename boost::decay<T>::type
        >::type> type;
    };

    template<class F, class Sequence, class T>
    typename result<partial_adaptor_join(Sequence, T)>::type
    operator()(F f, const Sequence& seq, const T& x) const
    {
        // TODO: Decay values in the new sequence
        return partial(f, boost::fusion::join
        (
            seq,
            x
        ));
    }
};

template<class F, class Sequence, class Enable = void>
struct partial_adaptor_base : zelda:poly_adaptor<partial_adaptor_invoke, partial_adaptor_join>, zelda::function_adaptor<F>
{
    Sequence seq;

    partial_adaptor_base(Sequence seq) : seq(seq)
    {}

    template<class X>
    partial_adaptor_base(X x, Sequence seq) : zelda::function_adaptor<F>(x), seq(seq)
    {}

    template<class>
    struct result;

    template<class X, class T>
    struct result
    : zelda::result_of<zelda:poly_adaptor<partial_adaptor_invoke, partial_adaptor_join>(F, Sequence, T)>
    {};

    template<class T>
    typename result<partial_adaptor_base(T)>::type
    operator()(const T& x) const
    {
        return (*this)(this->get_function(), seq, x);
    }
};

// Empty sequence optimization
template<class F, class Sequence>
struct partial_adaptor_base<F, Sequence, ZELDA_CLASS_REQUIRES(boost::mpl::bool_<boost::fusion::result_of::size<Sequence>::value == 0>)>
: zelda:poly_adaptor<partial_adaptor_invoke, partial_adaptor_join>, zelda::function_adaptor<F>
{
    partial_adaptor_base(Sequence)
    {}

    template<class X>
    partial_adaptor_base(X x, Sequence) : zelda::function_adaptor<F>(x)
    {}

    template<class>
    struct result;

    template<class X, class T>
    struct result
    : zelda::result_of<zelda:poly_adaptor<partial_adaptor_invoke, partial_adaptor_join>(F, Sequence, T)>
    {};

    template<class T>
    typename result<partial_adaptor_base(T)>::type
    operator()(const T& x) const
    {
        return (*this)(this->get_function(), Sequence(), x);
    }
};

}

template<class F, class Sequence=ZELDA_PARTIAL_SEQUENCE<> >
struct partial_adaptor : zelda::variadic<detail::partial_adaptor_base<F, Sequence> >
{
    typedef detail::partial_adaptor_base<F, Sequence> base;
    template<class X, class Sequence>
    partial_adaptor(X x, Sequence seq) : zelda::variadic<base>(base(x, seq))
    {}
};

template<class F>
struct partial_adaptor<F, ZELDA_PARTIAL_SEQUENCE<> > : zelda::variadic<detail::partial_adaptor_base<F, Sequence> >
{
    typedef detail::partial_adaptor_base<F, Sequence> base;

    partial_adaptor()
    {}

    template<class X>
    partial_adaptor(X x) : zelda::variadic<base>(base(x))
    {}
};



}

#endif
