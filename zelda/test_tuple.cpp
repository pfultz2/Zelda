

#include <cstdio>
#include <zelda/tuple.h>

using namespace zelda;

#define ZELDA_TEST_CHECK(x, ...) \
if ((__VA_ARGS__) != x) printf("FAILED@%i: %s\n", __LINE__, #__VA_ARGS__);

// namespace boost { namespace tuples {

// template <class T> struct access_traits<T&&> {

//   typedef T&& const_type;
//   typedef T&& non_const_type;

//   typedef T&& parameter_type;
// };

// }}

struct foo
{
    template<class T>
    void operator()(T& x) const
    {
        x++;
    }

    template<class T, class U>
    void operator()(T& x, U y) const
    {
        x += y;
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
    ZELDA_FORWARD_REF(T) operator()(ZELDA_FORWARD_REF(T) x) const
    {
        return zelda::forward<T>(x);
    }

};

int main()
{

    //
    // forward_as_tuple
    //
    int i = 1;
    tuple<int&> t1 = forward_as_tuple(i);
    // test_get(t1);
    // int& ii = get<0>(t1);

    //
    // tuple_cat
    //
    tuple<int, int> tc1 = tuple_cat(make_tuple(0), make_tuple(1));
    ZELDA_TEST_CHECK(0, get<0>(tc1));
    ZELDA_TEST_CHECK(1, get<1>(tc1));

    tuple<int&, int> t2 = tuple_cat(t1, make_tuple(-1));
    ZELDA_TEST_CHECK(1, get<0>(t2));
    ZELDA_TEST_CHECK(-1, get<1>(t2));

    //
    // invoke
    //
    ZELDA_TEST_CHECK(2, invoke(unary_class(), make_tuple(2)));
    invoke(foo(), t1);
    ZELDA_TEST_CHECK(2, i);
    invoke(foo(), t2);
    ZELDA_TEST_CHECK(1, i);
    
}