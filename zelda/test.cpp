

#include <cstdio>
//#include <boost/fusion/functional/invocation/invoke.hpp>
//#include <boost/fusion/adapted/std_tuple.hpp>
//#include "invoke.h"
#include <zelda/pipable.h>

#define ZELDA_TEST_CHECK(x, ...) \
if ((__VA_ARGS__) != x) printf("FAILED@%i: %s\n", __LINE__, #__VA_ARGS__);

struct binary_class
{
    template<class F>
    struct result;

    template<class F, class T, class U>
    struct result<F(T, U)>
    {
        typedef T type;
    };

    template<class T, class U>
    T operator()(T x, U y) const
    {
        return x+y;
    }

};

struct mutable_class
{
    template<class F>
    struct result;

    template<class F, class T, class U>
    struct result<F(T, U)>
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
        typedef T type;
    };

    template<class T>
    T operator()(T x) const
    {
        return x;
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

zelda::forward_adaptor<void_class> void_forward;

zelda::pipable_adaptor<binary_class> binary_pipable;
//zelda::partial_adaptor<binary_class> binary_partial = {};

zelda::pipable_adaptor<unary_class> unary_pipable;
//zelda::partial_adaptor<unary_class> unary_partial = {};

zelda::pipable_adaptor<mutable_class> mutable_pipable;

zelda::pipable_adaptor<void_class> void_pipable;

mutable_class foo = {};


// struct static_class
// {
//     constexpr static_class() {}
//     void operator()() const
//     {
//         printf("Static class\n");
//     }
// };

// struct model_class
// {
//     static static_class const static_ = static_class();
// };

int main()
{

    void_forward(1);

    void_pipable(1);
    // 1 | void_pipable;
    // pipable
    // ZELDA_TEST_CHECK(3, 1 | binary_pipable(2));
    // ZELDA_TEST_CHECK(3, binary_pipable(1, 2));
    // ZELDA_TEST_CHECK(3, 3 | unary_pipable);
    // ZELDA_TEST_CHECK(3, unary_pipable(3));
    // int i1 = 1;
    // ZELDA_TEST_CHECK(3, 2 | binary_pipable(i1));
    // ZELDA_TEST_CHECK(3, i1 | mutable_pipable(2));
    // int i2 = 1;
    // ZELDA_TEST_CHECK(3, mutable_pipable(i2, 2));

    //partial
    // ZELDA_TEST_CHECK(3, binary_partial(1)(2));
    // ZELDA_TEST_CHECK(3, binary_partial(1, 2));
    // ZELDA_TEST_CHECK(3, unary_partial()(3));
    // ZELDA_TEST_CHECK(3, unary_partial(3));
}