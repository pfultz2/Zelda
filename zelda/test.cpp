

#include <cstdio>
//#include <boost/fusion/functional/invocation/invoke.hpp>
//#include <boost/fusion/adapted/std_tuple.hpp>
//#include "invoke.h"
#include "function.h"

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
    template<class T>
    void operator()(T) const
    {
    }
};

zelda::pipable_adaptor<binary_class> binary_pipable = {};
zelda::partial_adaptor<binary_class> binary_partial = {};

zelda::pipable_adaptor<unary_class> unary_pipable = {};
zelda::partial_adaptor<unary_class> unary_partial = {};

zelda::pipable_adaptor<mutable_class> mutable_pipable = {};

zelda::pipable_adaptor<void_class> void_pipable = {};

mutable_class foo = {};

//static_assert(boost::fusion::traits::is_sequence<zelda::tuple<int, int> >::value, "Failed");

// template<class T>
// int test_get(T && x)
// {
//     return zelda::get<0>(x);
// }

// namespace test_detail {
// template<class F, class T, int ...N>
// int invoke_impl(F f, T && t, zelda::detail::seq<N...>) 
// {
//     return f(zelda::get<N>(std::forward<T>(t))...);
// }

// template<class F, class Tuple>
// int invoke(F f, Tuple && t)
// {
//     return test_detail::invoke_impl(f, std::forward<Tuple>(t), typename zelda::detail::gens<zelda::tuple_size<Tuple>::value>::type() );
// }
// }

template <typename T>
void bar(T const& t)
{
    //std::move(zelda::get<0>(std::forward<const T>(t))) = 123;
    //std::move(zelda::get<0>(t)) = 123;
    //zelda::get<0>(std::forward<const T>(t)) = 123;
    // std::forward<typename zelda::tuple_element<0, const T&>::type>(zelda::get<0>(std::forward<const T&>(t))) = 123;
    // static_assert(std::is_same<decltype(zelda::get<0>(std::forward<const T>(t))), 
    //     typename zelda::tuple_element<0, T>::type>::value, 
    //     "Its different");
    // //std::forward<typename zelda::tuple_element<0, T>::type>(zelda::get<0>(t)) = 123;
    // auto tt = zelda::make_tuple(1, 2);
    // auto z = zelda::tuple_cat(t, tt);
}

int main()
{
    // auto t1 = zelda::forward_as_tuple(1,2);
    // auto t2 = zelda::tuple<>();
    // zelda::invoke(binary_class(), zelda::tuple_cat(t2, std::move(t1)));
    // int i = 999;
    // zelda::tuple<int&, int> t(i, 1);
    // bar(t);
    //auto s =  zelda::forward_as_tuple(1,2);
    //int && i = std::forward<int>(zelda::get<0>(zelda::forward_as_tuple(1,2)));
    //printf("%i\n", i);
    //printf("%i\n", zelda::invoke(binary_class(), s));
    //int i = zelda::details::invoke_impl(foo_t(), boost::fusion::begin(s), boost::fusion::end(s));
    //printf("%i\n", zelda::invoke(binary_class(), zelda::forward_as_tuple(1,2)));
    void_pipable(1);
    1 | void_pipable;
    //pipable
    ZELDA_TEST_CHECK(3, 1 | binary_pipable(2));
    ZELDA_TEST_CHECK(3, binary_pipable(1, 2));
    ZELDA_TEST_CHECK(3, 3 | unary_pipable);
    ZELDA_TEST_CHECK(3, unary_pipable(3));
    int i1 = 1;
    ZELDA_TEST_CHECK(3, 2 | binary_pipable(i1));
    ZELDA_TEST_CHECK(3, i1 | mutable_pipable(2));
    int i2 = 1;
    ZELDA_TEST_CHECK(3, mutable_pipable(i2, 2));

    //partial
    ZELDA_TEST_CHECK(3, binary_partial(1)(2));
    ZELDA_TEST_CHECK(3, binary_partial(1, 2));
    ZELDA_TEST_CHECK(3, unary_partial()(3));
    ZELDA_TEST_CHECK(3, unary_partial(3));
}