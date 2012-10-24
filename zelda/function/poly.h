/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    poly.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_POLY_H
#define ZELDA_GUARD_FUNCTION_POLY_H

#include <zelda/function/adaptor.h>
#include <zelda/function/variadic.h>
#include <zelda/function/fuse.h>
#include <zelda/is_callable.h>

#ifndef ZELDA_POLY_LIMIT
#define ZELDA_POLY_LIMIT 16
#endif

namespace zelda {

namespace detail {

template<class F1, class F2, class Enable = void>
struct poly_kernel_base
{
    F1 f1;
    F2 f2;
    poly_kernel_base() {}

    template<class A, class B>
    poly_kernel_base(A f1, B f2) : f1(f1), f2(f2)
    {}

    F1 get_function1() const
    {
        return f1;
    }
    F2 get_function2() const
    {
        return f2;
    }
};

template<class F1, class F2>
struct poly_kernel_base<F1, F2, ZELDA_CLASS_REQUIRES(boost::is_empty<F1>, boost::is_empty<F2>)>
{
    poly_kernel_base() {}

    template<class A, class B>
    poly_kernel_base(A, B)
    {}

    F1 get_function1() const
    {
        return F1();
    }
    F2 get_function2() const
    {
        return F2();
    }
};

template<class F1, class F2>
struct poly_kernel : poly_kernel_base<F1, F2>
{
    poly_kernel() {}

    template<class A, class B>
    poly_kernel(A f1, B f2) : poly_kernel_base<F1, F2>(f1, f2)
    {}

    template<class X>
    struct result;

    template<class X, class T>
    struct result<X(T)>
    : boost::mpl::eval_if
    <
        is_callable<F1(ZELDA_FORWARD_REF(T))>, 
        zelda::result_of<F1(ZELDA_FORWARD_REF(T))>, 
        zelda::result_of<F2(ZELDA_FORWARD_REF(T))> 
    >
    {};

    template<class T>
    typename zelda::result_of<F1(ZELDA_FORWARD_REF(T))>::type 
    operator()(ZELDA_FORWARD_REF(T) t) const
    {
        return this->get_function1()(zelda::forward<T>(t));
    }

    template<class T>
    ZELDA_FUNCTION_REQUIRES(exclude is_callable<F1(ZELDA_FORWARD_REF(T))>)
    (typename zelda::result_of<F2(ZELDA_FORWARD_REF(T))>::type)
    operator()(ZELDA_FORWARD_REF(T) t) const
    {
        return this->get_function2()(zelda::forward<T>(t));
    }
};

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
template<class F, class... Fs>
struct poly_base : poly_kernel<F, poly_base<Fs...> >
{
    poly_base() {}

    template<class X, class... Xs>
    poly_base(X f1, Xs... fs) : poly_kernel<F, poly_base<Fs...> >(f1, poly_base<Fs...>(fs...))
    {}
};

#else
template<class T = void, ZELDA_PP_PARAMS_Z(1, ZELDA_POLY_LIMIT, class T, = void BOOST_PP_INTERCEPT)>
struct poly_base;
#define ZELDA_POLY_BASE(z, n, data) \
template<class F, ZELDA_PP_PARAMS_Z(z, n, class Fs)> \
struct poly_base<F, ZELDA_PP_PARAMS_Z(z, n, Fs)> \
: poly_kernel<F, poly_base<ZELDA_PP_PARAMS_Z(z, n, Fs)> > \
{ \
    poly_base() {} \
\
    template<class X, ZELDA_PP_PARAMS_Z(z, n, class Xs)> \
    poly_base(X f1, ZELDA_PP_PARAMS_Z(z, n, Xs, fs)) \
    : poly_kernel<F, poly_base<ZELDA_PP_PARAMS_Z(z, n, Fs)> >(f1, poly_base<ZELDA_PP_PARAMS_Z(z, n, Fs)>(ZELDA_PP_PARAMS_Z(z, n, fs))) \
    {} \
};
BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_POLY_LIMIT, ZELDA_POLY_BASE, ~)
#endif

template<class F>
struct poly_base<F> : function_adaptor_base<F>
{
    poly_base() {}

    template<class X>
    poly_base(X f) : function_adaptor_base<F>(f)
    {}

    template<class T>
    typename zelda::result_of<F(ZELDA_FORWARD_REF(T))>::type 
    operator()(ZELDA_FORWARD_REF(T) t) const
    {
        return this->get_function()(zelda::forward<T>(t));
    }
};

}

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
template<class... Fs>
struct poly_adaptor : variadic_adaptor<detail::poly_base<fuse_adaptor<Fs>...> >
{
    poly_adaptor() {}

    template<class... X>
    poly_adaptor(X... fs) : variadic_adaptor<detail::poly_base<fuse_adaptor<Fs>...> >(detail::poly_base<fuse_adaptor<Fs>...>(fs...))
    {}
};

template<class... Fs>
poly_adaptor<Fs...> poly(Fs... fs)
{
    return poly_adaptor<Fs...>(fs...);
}
#else
namespace detail {

template<class F>
struct poly_fuse
{
    typedef fuse_adaptor<F> type;
};

template<>
struct poly_fuse<void>
{
    typedef void type;
};

}


template<ZELDA_PP_PARAMS_Z(1, ZELDA_POLY_LIMIT, class Fs, = void BOOST_PP_INTERCEPT)>
struct poly_adaptor 
: variadic_adaptor<detail::poly_base<ZELDA_PP_PARAMS_Z(1, ZELDA_POLY_LIMIT, typename detail::poly_fuse<Fs,>::type BOOST_PP_INTERCEPT)> >
{
    poly_adaptor() {}

#define ZELDA_POLY_ADAPTOR_CONSTRUTOR(z, n, data)
    template<ZELDA_PP_PARAMS_Z(z, n, class X)> \
    poly_adaptor(ZELDA_PP_PARAMS_Z(z, n, X, fs)) \
    : variadic_adaptor<detail::poly_base<ZELDA_PP_PARAMS_Z(1, ZELDA_POLY_LIMIT, typename detail::poly_fuse<Fs,>::type BOOST_PP_INTERCEPT)> > \
    (detail::poly_base<ZELDA_PP_PARAMS_Z(z, ZELDA_POLY_LIMIT, typename detail::poly_fuse<Fs,>::type BOOST_PP_INTERCEPT)>(ZELDA_PP_PARAMS_Z(z, n, fs))) \
    {}
    BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_POLY_LIMIT, ZELDA_POLY_ADAPTOR_CONSTRUTOR, ~)
};

#define ZELDA_POLY_FUNCTION(z, n, data) \
template<ZELDA_PP_PARAMS_Z(z, n, class Fs)> \
poly_adaptor<ZELDA_PP_PARAMS_Z(z, n, Fs)> poly(ZELDA_PP_PARAMS_Z(z, n, Fs, fs)) \
{ \
    return poly_adaptor<ZELDA_PP_PARAMS_Z(z, n, Fs)>(ZELDA_PP_PARAMS_Z(z, n, fs)); \
}
BOOST_PP_REPEAT_1(1, ZELDA_POLY_LIMIT, ~)

#endif

}

#endif
