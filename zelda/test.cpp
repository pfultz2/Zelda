

#include <cstdio>
#include "function.h"

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
    T operator()(T x, U y)
    {
        return x+y;
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
    T operator()(T x)
    {
        return x;
    }

};

zelda::pipable_adaptor<binary_class> binary_pipable = {};
zelda::partial_adaptor<binary_class> binary_partial = {};

zelda::pipable_adaptor<unary_class> unary_pipable = {};
zelda::partial_adaptor<unary_class> unary_partial = {};

//static_assert(boost::fusion::traits::is_sequence<zelda::tuple<int, int> >::value, "Failed");

int main()
{
    //auto s =  zelda::make_tuple(1,2);
    //int i = zelda::details::invoke_impl(foo_t(), boost::fusion::begin(s), boost::fusion::end(s));
    printf("%i\n", zelda::invoke(binary_class(), zelda::make_tuple(1,2)));
    //pipable
    printf("%i\n", 1 | binary_pipable(2));
    printf("%i\n", binary_pipable(1, 2));
    printf("%i\n", 3 | unary_pipable);
    printf("%i\n", unary_pipable(3));

    //partial
    //printf("%i\n", binary_partial(1)(2));
    printf("%i\n", binary_partial(1, 2));
    //printf("%i\n", unary_partial()(3));
    printf("%i\n", unary_partial(3));
}