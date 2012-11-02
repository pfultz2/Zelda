

#include <cstdio>
//#include <boost/fusion/functional/invocation/invoke.hpp>
//#include <boost/fusion/adapted/std_tuple.hpp>
//#include "invoke.h"
#include <zelda/function/pipable.h>
#include <zelda/function/static.h>
#include <zelda/function/defer.h>

#include <boost/fusion/container/generation/make_vector.hpp>

namespace boost { namespace fusion { namespace detail
{

#ifndef ZELDA_NO_RVALUE_REFS
    template <typename T>
    struct call_param<T &&>
    {
        typedef T&& type;
    };
#endif
}}}

#define ZELDA_TEST_CHECK(x, ...) \
if ((__VA_ARGS__) != x) printf("FAILED@%i: %s\n", __LINE__, #__VA_ARGS__);

struct binary_class_d
{
    // template<class F>
    // struct result;

    // template<class F, class T, class U>
    // struct result<F(T, U)>
    // {
    //     typedef ZELDA_XTYPEOF_TPL(zelda::declval<T>() + zelda::declval<U>()) type;
    //     //typedef T type;
    // };

    template<class T, class U>
    T operator()(T x, U y) const
    {
        return x+y;
    }

};

typedef zelda::defer_adaptor<binary_class_d> binary_class;

struct mutable_class
{
    template<class F>
    struct result;

    template<class F, class T, class U>
    struct result<F(T&, U)>
    {
        typedef T type;
    };

    template<class T, class U>
    T operator()(T & x, U y) const
    {
        return x+=y;
    }

};

struct unary_class
{
    template<class F>
    struct result;

    template<class F, class T>
    struct result<F(T)>
    {
        typedef typename zelda::add_forward_reference<T>::type type;
    };

    template<class T>
    ZELDA_FORWARD_REF(T) operator()(ZELDA_FORWARD_REF(T) x) const
    {
        return zelda::forward<T>(x);
    }

};

struct void_class
{
    template<class F>
    struct result;

    template<class F, class T>
    struct result<F(T)>
    {
        typedef void type;
    };

    template<class T>
    void operator()(T) const
    {
    }
};

struct mono_class
{
    template<class F>
    struct result;

    template<class F, class T>
    struct result<F(T)>
    {
        typedef int type;
    };

    int operator()(int x) const
    {
        return x+1;
    }
};

struct tuple_class
{
    template<class F>
    struct result;

    template<class F, class T>
    struct result<F(T)>
    {
        typedef int type;
    };

    template<class T>
    int operator()(T t) const
    {
        return boost::fusion::at_c<0>(t) + 1;
    }
};

typedef zelda::variadic_adaptor<tuple_class> vard_class;
typedef zelda::perfect_adaptor<tuple_class> perfect_class;

typedef zelda::perfect_adaptor<binary_class> binary_perfect;

typedef zelda::perfect_adaptor<void_class> void_perfect;

typedef zelda::perfect_adaptor<mono_class> mono_perfect;

zelda::static_<binary_class> binary_static = {};

zelda::static_<void_class> void_static = {};

zelda::static_<mono_class> mono_static = {};

zelda::static_<zelda::fuse_adaptor<unary_class> > unary_fuse = {};

zelda::static_<zelda::pipable_adaptor<binary_class> > binary_pipable = {};

zelda::static_<zelda::pipable_adaptor<unary_class> > unary_pipable = {};

zelda::static_<zelda::pipable_adaptor<mutable_class> > mutable_pipable = {};

zelda::static_<zelda::pipable_adaptor<void_class> > void_pipable = {};

zelda::static_<zelda::pipable_adaptor<mono_class> > mono_pipable = {};

// mutable_class foo = {};

int main()
{
    // perfect
    void_perfect()(1);
    ZELDA_TEST_CHECK(3, binary_perfect()(1, 2));
    ZELDA_TEST_CHECK(3, mono_perfect()(2));

    // static
    void_static(1);
    ZELDA_TEST_CHECK(3, binary_static(1, 2));
    ZELDA_TEST_CHECK(3, mono_static(2));
    // variadic
    ZELDA_TEST_CHECK(3, vard_class()(2));
    // fuse
    int ifu = 3;
    ZELDA_TEST_CHECK(3, zelda::fuse(unary_class())(boost::fusion::make_vector(3)));
    ZELDA_TEST_CHECK(3, unary_fuse(boost::fusion::make_vector(3)));
    ZELDA_TEST_CHECK(3, unary_fuse(boost::fusion::vector<int&>(ifu)));
    // pipable
    void_pipable(1);
    1 | void_pipable;
    ZELDA_TEST_CHECK(3, 1 | binary_pipable(2));
    ZELDA_TEST_CHECK(3, binary_pipable(1, 2));
    ZELDA_TEST_CHECK(3, 3 | unary_pipable);
    ZELDA_TEST_CHECK(3, unary_pipable(3));
    int i1 = 1;
    ZELDA_TEST_CHECK(3, 2 | binary_pipable(i1));
    ZELDA_TEST_CHECK(3, i1 | mutable_pipable(2));
    int i2 = 1;
    ZELDA_TEST_CHECK(3, mutable_pipable(i2, 2));
    ZELDA_TEST_CHECK(3, mono_pipable(2));
    ZELDA_TEST_CHECK(3, 2| mono_pipable);

    //partial
    // ZELDA_TEST_CHECK(3, binary_partial(1)(2));
    // ZELDA_TEST_CHECK(3, binary_partial(1, 2));
    // ZELDA_TEST_CHECK(3, unary_partial()(3));
    // ZELDA_TEST_CHECK(3, unary_partial(3));
}