/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    regular.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_REGULAR_H
#define ZELDA_GUARD_FUNCTION_REGULAR_H

#include <zelda/function/adaptor.h>
#include <zelda/function/perfect.h>
#include <zelda/function/variadic.h>
#include <zelda/function/invoke.h>

namespace zelda { namespace detail {


template<class F>
struct regular_base
{
    boost::optional<F> f;

    typedef F function_type;

    regular_base()
    {}
    template<class X>
    regular_base(X f): f(f)
    {}

    regular_base(const regular_base & rhs) : f(rhs.f)
    {}

    // Assignment operator is just a copy construction, which does not provide
    // the strong exception guarentee.
    regular_base& operator=(const regular_base& rhs)
    {
        if (this != &rhs)
        {
            this->~regular_base();
            new (this) regular_base(rhs);
        }
        return *this;
    }

    template<class X, class Enable = void>
    struct result;

    template<class X, class T>
    struct result<X(T), ZELDA_CLASS_REQUIRES(boost::fusion::traits::is_sequence<typename boost::decay<T>::type>)>
    : invoke_result<F, const typename boost::decay<T>::type&> 
    {}; 

    template<class T>
    typename result<F(const T&)>::type operator()(const T & x) const
    {
        return invoke(this->f, x);
    }
};
}

template<class F>
struct regular_adaptor : zelda::variadic_adaptor<detail::regular_base<F> >
{
    regular_adaptor()
    {}

    template<class X>
    regular_adaptor(X x) : zelda::variadic_adaptor<detail::regular_base<F> >(x)
    {}
};

template<class F>
regular_adaptor<F> regular(F f)
{
    return regular_adaptor<F>(f);
}

}

#ifdef ZELDA_TEST
#include <zelda/test.h>
#include <zelda/function/placeholders.h>
#include <algorithm>
#include <boost/iterator/filter_iterator.hpp>

ZELDA_TEST_CASE(regular_test)
{
    int a1[6] = {1,2,3,4,5,6};
    int a2[3] = {2,4,6};

    ZELDA_TEST_CHECK(
        std::equal(a2, a2+3,
            boost::make_filter_iterator(regular(ph::_1 % 2 == 0), a1, a1+6) )
    );
}

#endif

#endif
