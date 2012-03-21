#ifndef ZELDA_MPL_H
#define ZELDA_MPL_H

#include "requires.h"

#include <boost/mpl/long.hpp>
#include <boost/mpl/at_fwd.hpp>
#include <boost/mpl/clear_fwd.hpp>
#include <boost/mpl/size_fwd.hpp>
#include <boost/mpl/push_back_fwd.hpp>
#include <boost/mpl/advance_fwd.hpp>
#include <boost/mpl/distance_fwd.hpp>
#include <boost/mpl/pop_back_fwd.hpp>
#include <boost/mpl/pop_front_fwd.hpp>
#include <boost/mpl/push_front_fwd.hpp>

#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/not_equal_to.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/prior.hpp>

#include <boost/type_traits/is_same.hpp>

namespace zelda {
namespace mpl
{

template<class T>
struct lazy : T {};

template<class T>
struct eval
{
    typedef T type;
};

template<class T>
struct eval<lazy<T> >
: T {};

namespace detail
{
struct no_else {};
}

template<bool C, class T = void, class E = detail::no_else>
struct if_c;

namespace detail
{

template<bool C, class T>
struct if_clause
{
    //typedef if_c o;

    template<bool C1, class T1 = void, class E1 = no_else>
    struct else_if_c
    : zelda::mpl::if_c<C || C1, typename zelda::mpl::if_c<C, T, T1>::type, E1> {};

    template<class C1, class T1 = void, class E1 = no_else>
    struct else_if: else_if_c<C1::value, T1, E1> {};

    template<class E>
    class else_: public zelda::mpl::if_c<C, T, E> {};
};

}

template<class T, class E>
struct if_c<false, T, E>
: eval<E> {};

template<class T, class E>
struct if_c<true, T, E>
: eval<T> {};

template<class T>
struct if_c<false, T, detail::no_else>
: detail::if_clause<false, T> {};

template<class T>
struct if_c<true, T, detail::no_else> : detail::if_clause<true, T>
{
    typedef T type;
};

template<class C, class T = void, class E = detail::no_else>
struct if_: if_c<C::value, T, E> {};



struct vector_tag {};

template< typename... Args >
struct vector 
{
    typedef vector_tag tag;
};

namespace details {

struct vector_iterator_tag {};

template< class Vector, long N >
struct vector_iterator
: boost::mpl::at<Vector, boost::mpl::long_<N> >  
{
    typedef vector_iterator_tag tag;
    static const long pos = N;
};

template<class Vector, class Iterator, class End, class Enable = void>
struct push_back_iterator
{
};

template<template <typename...> class Vector, class Iterator, class End, class... Args>
struct push_back_iterator<Vector<Args...>, Iterator, End, ZELDA_CLASS_REQUIRES(boost::is_same<Iterator, End>)>
{
    typedef Vector<Args...> type;
};

template<template <typename...> class Vector, class Iterator, class End, class... Args>
struct push_back_iterator<Vector<Args...>, Iterator, End, ZELDA_CLASS_REQUIRES(ZELDA_EXCLUDE(boost::is_same<Iterator, End>))>
: push_back_iterator<Vector<Args..., typename boost::mpl::deref<Iterator>::type>, typename boost::mpl::next<Iterator>::type, End>
{};
}
}
}

namespace boost{ namespace mpl {

template<>
struct at_impl< zelda::mpl::vector_tag >
{
    template<class TypeList, int Index> 
    struct apply_c {};

    template<class TypeList, class Index>
    struct apply
    {
        typedef typename apply_c<TypeList, Index::value>::type type;
    };
    //TODO: Check to see if value is in range

    template<template <typename...> class List, class Head, class... Args>
    struct apply_c<List<Head, Args...>, 0>
    {
        typedef Head type;
    };

    template<int Index, template <typename...> class List, class Head, class... Args>
    struct apply_c<List<Head, Args...>, Index>
    {
        typedef typename apply_c<List<Args...>, Index-1>::type type;
    };
};


template<>
struct begin_impl< zelda::mpl::vector_tag >
{
    template< typename Vector > struct apply
    {
        typedef zelda::mpl::details::vector_iterator<Vector,0> type;
    };
};

template<>
struct end_impl< zelda::mpl::vector_tag >
{
    template< typename Vector > struct apply
    {};

    template<template <typename...> class Vector, class... Args>
    struct apply<Vector<Args...>>
    {
        typedef zelda::mpl::details::vector_iterator<Vector<Args...>,sizeof...(Args)> type;
    };
};

template<class Vector, long N>
struct next< zelda::mpl::details::vector_iterator<Vector,N> >
{
    typedef zelda::mpl::details::vector_iterator<Vector,(N + 1)> type;
};

template<class Vector, long N>
struct prior< zelda::mpl::details::vector_iterator<Vector,N> >
{
    typedef zelda::mpl::details::vector_iterator<Vector,(N - 1)> type;
};

template<class Vector, long N, class Distance>
struct advance< zelda::mpl::details::vector_iterator<Vector,N>, Distance >
{
    typedef zelda::mpl::details::vector_iterator<Vector,(N + Distance::value)> type;
};

template<class Vector, long N, long M>
struct distance< zelda::mpl::details::vector_iterator<Vector,N>, zelda::mpl::details::vector_iterator<Vector,M> >
: mpl::long_<(M - N)> {};

template<>
struct clear_impl< zelda::mpl::vector_tag >
{
    template< typename Vector > struct apply
    {
        typedef zelda::mpl::vector<> type;
    };
};

template<>
struct size_impl< zelda::mpl::vector_tag >
{
    template<class TypeList>
    struct apply {};

    template<template <typename...> class List, class... Args>
    struct apply<List<Args...>> 
    : boost::mpl::long_<sizeof... (Args)> {};
};

template<>
struct pop_back_impl< zelda::mpl::vector_tag >
{
    template< typename Vector > 
    struct apply {};

    template<template <typename...> class Vector, class... Args>
    struct apply<Vector<Args...>>
    : zelda::mpl::details::push_back_iterator<Vector<>, 
    typename boost::mpl::begin<Vector<Args...>>::type, 
    typename boost::mpl::prior<typename boost::mpl::end<Vector<Args...>>::type>::type>
    {};
};

template<>
struct pop_front_impl< zelda::mpl::vector_tag >
{
    template< typename Vector > 
    struct apply {};

    template<template <typename...> class Vector, class Head, class... Args>
    struct apply<Vector<Head, Args...>>
    {
        typedef Vector<Args...> type;
    };
};

template<>
struct push_back_impl< zelda::mpl::vector_tag >
{
    template< typename Vector, typename T > 
    struct apply {};

    template<template <typename...> class List, class Type, class... Args>
    struct apply<List<Args...>, Type>
    {
        typedef List<Args..., Type> type;
    };
};

template<>
struct push_front_impl< zelda::mpl::vector_tag >
{
    template< typename Vector, typename T > 
    struct apply {};

    template<template <typename...> class List, class Type, class... Args>
    struct apply<List<Args...>, Type>
    {
        typedef List<Type, Args...> type;
    };
};

}}

#ifdef ZELDA_TEST
#include <boost/mpl/at.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/push_front.hpp>
namespace zelda{namespace test_mpl{

typedef zelda::mpl::vector< boost::mpl::long_<1>, boost::mpl::long_<2>, boost::mpl::long_<3> > vector;

static_assert(boost::mpl::at<vector, boost::mpl::long_<1>>::type::value == boost::mpl::long_<2>::value, "Failed");
static_assert(boost::mpl::back<vector>::type::value == boost::mpl::long_<3>::value, "Failed");
static_assert(boost::mpl::front<vector>::type::value == boost::mpl::long_<1>::value, "Failed");
//pop_back
static_assert(boost::mpl::back<typename boost::mpl::pop_back<vector>::type>::type::value == boost::mpl::long_<2>::value, "Failed");
static_assert(boost::mpl::front<typename boost::mpl::pop_back<vector>::type>::type::value == boost::mpl::long_<1>::value, "Failed");
//pop_front
static_assert(boost::mpl::back<typename boost::mpl::pop_front<vector>::type>::type::value == boost::mpl::long_<3>::value, "Failed");
static_assert(boost::mpl::front<typename boost::mpl::pop_front<vector>::type>::type::value == boost::mpl::long_<2>::value, "Failed");
//push_back
static_assert(boost::mpl::back<typename boost::mpl::push_back<vector, boost::mpl::long_<4>>::type>::type::value == boost::mpl::long_<4>::value, "Failed");
static_assert(boost::mpl::front<typename boost::mpl::push_back<vector, boost::mpl::long_<4>>::type>::type::value == boost::mpl::long_<1>::value, "Failed");
//push_front
static_assert(boost::mpl::back<typename boost::mpl::push_front<vector, boost::mpl::long_<0>>::type>::type::value == boost::mpl::long_<3>::value, "Failed");
static_assert(boost::mpl::front<typename boost::mpl::push_front<vector, boost::mpl::long_<0>>::type>::type::value == boost::mpl::long_<0>::value, "Failed");

}
}


#endif

#endif