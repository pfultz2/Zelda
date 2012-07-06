/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    typeof.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_TYPEOF_H
#define	ZELDA_GUARD_TYPEOF_H


#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

#ifndef ZELDA_NO_DECLTYPE
#define ZELDA_TYPEOF decltype
#define ZELDA_TYPEOF_TPL decltype
#elif ZELDA_HAS_TYPEOF
#define ZELDA_TYPEOF __typeof__
#define ZELDA_TYPEOF_TPL __typeof__
#else
#include <boost/typeof/typeof.hpp>
#define ZELDA_TYPEOF BOOST_TYPEOF
#define ZELDA_TYPEOF_TPL BOOST_TYPEOF_TPL
#endif

#ifndef ZELDA_NO_AUTO
#define ZELDA_AUTO(var, ...) auto var = __VA_ARGS__
#else
#define ZELDA_AUTO(var, ...) ZELDA_TYPEOF(__VA_ARGS__) var = __VA_ARGS__
#endif

#ifndef ZELDA_NO_TRAILING_RETURN
    #ifndef ZELDA_NO_NOEXCEPT
    #define ZELDA_RETURNS(...) \
        noexcept(noexcept(decltype(__VA_ARGS__)(std::move(__VA_ARGS__)))) \
        -> decltype(__VA_ARGS__) \
        { return (__VA_ARGS__); }
    #else
    #define ZELDA_RETURNS(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }
    #endif
#else
#define ZELDA_RETURNS(...) ZELDA_ERROR_TRAILING_RETURNS_NOT_SUPPORTED
#endif

#ifndef ZELDA_NO_DECLTYPE
#define ZELDA_XTYPEOF(...) decltype((__VA_ARGS__))
#define ZELDA_XTYPEOF_TPL(...) decltype((__VA_ARGS__))
#else
#define ZELDA_XTYPEOF(...) zelda::typeof_detail::xtypeof_<ZELDA_TYPEOF(x), ZELDA_TYPEOF_IS_LVALUE(x), ZELDA_TYPEOF_IS_RVALUE(x)>::type
#define ZELDA_XTYPEOF_TPL(...) typename zelda::typeof_detail::xtypeof_<ZELDA_TYPEOF_TPL(x), ZELDA_TYPEOF_IS_LVALUE_TPL(x), ZELDA_TYPEOF_IS_RVALUE_TPL(x)>::type
#endif

namespace zelda {

#ifndef ZELDA_NO_RVALUE_REFS
template<class T>
T&& declval();
#else
template<class T>
T declval();
#endif

namespace typeof_detail {
struct void_ {};
template<class T> const T& operator,(T const&, void_);
template<class T> T& operator,(T&, void_);
#define ZELDA_AVOID(x) ((x), zelda::typeof_detail::void_())

//rvalue probe from Eric Niebler
template<typename T>
struct rvalue_probe
{
    struct private_type_ {};
    // can't ever return an array by value
    typedef BOOST_DEDUCED_TYPENAME boost::mpl::if_<
        boost::mpl::or_<boost::is_abstract<T>, boost::is_array<T> >, private_type_, T
    >::type value_type;
    operator value_type();
    operator const T &() const;
};

template<typename T>
rvalue_probe<T> const make_probe(T const &);
template<typename T>
void_ const make_probe(void_);

#define ZELDA_TYPEOF_RVALUE_PROBE(x) true ? zelda::typeof_detail::make_probe(BSI_TYPEOF_AVOID(x)) : (x)

#define ZELDA_TYPEOF_IS_RVALUE(x) \
boost::mpl::and_<boost::mpl::not_<boost::is_array<ZELDA_TYPEOF(x)> >,\
boost::mpl::not_<boost::is_const<ZELDA_TYPEOF(BSI_TYPEOF_RVALUE_PROBE(x))> > >

#define ZELDA_TYPEOF_IS_RVALUE_TPL(x) \
boost::mpl::and_<boost::mpl::not_<boost::is_array<ZELDA_TYPEOF_TPL(x)> >,\
boost::mpl::not_<boost::is_const<ZELDA_TYPEOF_TPL(BSI_TYPEOF_RVALUE_PROBE(x))> > >

template<class T> boost::mpl::false_ is_lvalue(const T &);
template<class T> boost::mpl::true_ is_lvalue(T&);
inline boost::mpl::false_ is_lvalue(void_);

#define BSI_TYPEOF_IS_LVALUE(x) \
ZELDA_TYPEOF(zelda::typeof_detail::is_lvalue(BSI_TYPEOF_AVOID(x)))

#define BSI_TYPEOF_IS_LVALUE_TPL(x) \
ZELDA_TYPEOF_TPL(zelda::typeof_detail::is_lvalue(BSI_TYPEOF_AVOID(x)))

template<class T, class IsLvalue, class IsRvalue>
struct xtypeof_
: boost::mpl::if_<boost::mpl::or_<IsLvalue, boost::mpl::not_<IsRvalue> >, 
typename boost::add_reference<T>::type, 
T> 
{};

template<class IsLvalue, class IsRvalue>
struct xtypeof_<void_, IsLvalue, IsRvalue>
{
    typedef void type;
};

//is_const2 that works with references
template<class T>
struct is_const2 : boost::is_const<typename boost::remove_reference<T>::type >
{
};

}
}

#ifdef ZELDA_TEST
namespace bsi {
namespace typeof_test {

int by_value();
int& by_ref();
const int& by_const_ref();
void by_void();

struct foo
{
    static int by_value();
    static int& by_ref();
    static const int& by_const_ref();
    static void by_void();
};

template<class T>
struct tester
{
    typedef ZELDA_XTYPEOF(T::by_value()) test2;

    //On MSVC, static_assert is broken, so we use BOOST_MPL_ASSERT instead.
    BOOST_MPL_ASSERT_NOT((boost::is_reference<ZELDA_XTYPEOF_TPL(T::by_value())>));
    BOOST_MPL_ASSERT_NOT((zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(T::by_value())>));

    BOOST_MPL_ASSERT((boost::is_reference<ZELDA_XTYPEOF(T::by_ref())>));
    BOOST_MPL_ASSERT_NOT((zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(T::by_ref())>));

    BOOST_MPL_ASSERT((boost::is_reference<ZELDA_XTYPEOF(T::by_const_ref())>));
    BOOST_MPL_ASSERT((zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(T::by_const_ref())>));

    BOOST_MPL_ASSERT((boost::is_same<ZELDA_XTYPEOF(T::by_void()), void>));
};

static tester<foo> tested;

}
}

static_assert(not boost::is_reference<ZELDA_XTYPEOF(zelda::typeof_test::by_value())>::value, "Failed");
static_assert(not zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(zelda::typeof_test::by_value())>::value, "Failed");

static_assert(boost::is_reference<ZELDA_XTYPEOF(zelda::typeof_test::by_ref())>::value, "Failed");
static_assert(not zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(zelda::typeof_test::by_ref())>::value, "Failed");

static_assert(boost::is_reference<ZELDA_XTYPEOF(zelda::typeof_test::by_const_ref())>::value, "Failed");
static_assert(zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(zelda::typeof_test::by_const_ref())>::value, "Failed");

static_assert(boost::is_same<ZELDA_XTYPEOF(zelda::typeof_test::by_void()), void>::value, "Failed");

#endif




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// #define ZELDA_TYPEOF decltype

// namespace zelda {
// template<class T>
// T&& declval();

// namespace typeof_detail {
// struct value {};
// struct rvalue {};
// struct const_lvalue {};
// struct lvalue {};

// //is_const2 that works with references
// template<class T>
// struct is_const2 : boost::is_const<typename boost::remove_reference<T>::type >
// {
// };

// template<class T>
// typename tpl::if_<std::is_rvalue_reference<T&&>, value>
//                                 ::template else_if<is_const2<T&&>, const_lvalue, lvalue >
//                             ::type test(T&&);

// void test(...);

// template<class Tag, class T>
// struct xtypeof {};

// template<class T>
// struct xtypeof<rvalue, T> : std::add_rvalue_reference<T> {};

// template<class T>
// struct xtypeof<lvalue, T> : std::add_lvalue_reference<T> {};

// template<class T>
// struct xtypeof<const_lvalue, T> : std::add_const< typename std::add_lvalue_reference<T>::type > {};

// template<class T>
// struct xtypeof<value, T> : tpl::identity<T> {};

// template<class T>
// struct xtypeof<void, T> : tpl::identity<void> {};
// }
// }

// #define ZELDA_XTYPEOF(...) \
// typename zelda::typeof_detail::xtypeof<ZELDA_TYPEOF(zelda::typeof_detail::test(__VA_ARGS__)), ZELDA_TYPEOF(__VA_ARGS__)>::type

// #ifdef ZELDA_TEST
// namespace zelda {
// namespace typeof_test {

// int by_value();
// int&& by_rvalue();
// int& by_ref();
// const int& by_const_ref();
// void by_void();



// }
// }

// BOOST_STATIC_ASSERT((not std::is_reference<ZELDA_XTYPEOF(zelda::typeof_test::by_value())>::value));
// BOOST_STATIC_ASSERT((not zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(zelda::typeof_test::by_value())>::value));

// BOOST_STATIC_ASSERT((std::is_rvalue_reference<ZELDA_XTYPEOF(zelda::typeof_test::by_rvalue())>::value));
// BOOST_STATIC_ASSERT((not zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(zelda::typeof_test::by_rvalue())>::value));

// BOOST_STATIC_ASSERT((std::is_lvalue_reference<ZELDA_XTYPEOF(zelda::typeof_test::by_ref())>::value));
// BOOST_STATIC_ASSERT((not zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(zelda::typeof_test::by_ref())>::value));

// BOOST_STATIC_ASSERT((std::is_lvalue_reference<ZELDA_XTYPEOF(zelda::typeof_test::by_const_ref())>::value));
// BOOST_STATIC_ASSERT((zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(zelda::typeof_test::by_const_ref())>::value));

// BOOST_STATIC_ASSERT((std::is_same<ZELDA_XTYPEOF(zelda::typeof_test::by_void()), void>::value));

// #endif

// #if 0

// #ifdef NETBEANS
// #define typeof decltype
// #endif

// #ifdef ZELDA_HAS_DECLTYPE
// #define typeof decltype
// #define typeof_tpl decltype
// #endif

// #ifndef ZELDA_HAS_DECLTYPE

// #if defined(__GNUC__) || defined(__clang__)
// #define typeof_tpl typeof

// #elif defined(_MSC_VER)
// #if _MSC_VER >= 1600
// #define typeof decltype
// #define typeof_tpl decltype
// #else
// #define typeof BOOST_TYPEOF
// #define typeof_tpl BOOST_TYPEOF_TPL
// #endif

// #else
// #define typeof BOOST_TYPEOF
// #define typeof_tpl BOOST_TYPEOF_TPL
// #endif

// #define ZELDA_TYPEOF typeof

// #endif

// namespace zelda {

// template<class T>
// T declval();


// namespace typeof_detail {

// template<class T>
// struct identity
// {
//     typedef T type;
// };

// template<class T>
// struct wrap
// {
//     typedef typename T::type type;
// };

// template<class T>
// boost::tpl::true_ is_const(const T& x);

// template<class T>
// boost::tpl::false_ is_const(T & x);

// template<class T>
// inline typename boost::is_array<T>::type is_array(T const &);

// template<class T>
// inline boost::tpl::false_ is_rvalue(T &, int);

// template<class T>
// inline boost::tpl::true_ is_rvalue(T const &, ...);

// //rvalue probe from Eric Niebler
// template<typename T>
// struct rvalue_probe
// {
//     struct private_type_ {};
//     // can't ever return an array by value
//     typedef BOOST_DEDUCED_TYPENAME boost::tpl::if_<
//         boost::tpl::or_<boost::is_abstract<T>, boost::is_array<T> >, private_type_, T
//     >::type value_type;
//     operator value_type() { return *reinterpret_cast<value_type *>(this); } // never called
//     operator T &() const { return *reinterpret_cast<T *>(const_cast<rvalue_probe *>(this)); } // never called
// };

// template<typename T>
// rvalue_probe<T> const make_probe(T const &)
// {
//     return rvalue_probe<T>();
// }

// template<class Cond, class T>
// struct add_const_if : boost::tpl::if_<Cond, typename boost::add_const<T>::type, T >
// {
// };

// template<class IsConst, class T>
// struct xtype_ref : add_const_if<IsConst, typename boost::add_reference<T>::type >
// {   
// };

// template<class T, class IsConst, class IsRvalue>
// struct xtype : boost::tpl::if_<IsRvalue, T, typename xtype_ref<IsConst, T>::type>
// {
// };

// #define ZELDA_TYPEOF_IS_RVALUE(x) \
// boost::tpl::and_<boost::tpl::not_<typeof(zelda::typeof_detail::is_array(x))>,\
// typeof(zelda::typeof_detail::is_rvalue(true ? zelda::typeof_detail::make_probe(x) : (x), 0))>

// #define ZELDA_TYPEOF_IS_RVALUE_TPL(x) \
// boost::tpl::and_<boost::tpl::not_<typeof_tpl(zelda::typeof_detail::is_array(x))>,\
// typeof_tpl(zelda::typeof_detail::is_rvalue(true ? zelda::typeof_detail::make_probe(x) : (x), 0))>

// #define ZELDA_TYPEOF_IS_CONST(x) \
// typeof(zelda::typeof_detail::is_const(x))

// #define ZELDA_TYPEOF_IS_CONST_TPL(x) \
// typeof_tpl(zelda::typeof_detail::is_const(x))

// #define ZELDA_XTYPEOF(x) \
// zelda::typeof_detail::xtype<typeof(x), ZELDA_TYPEOF_IS_CONST(x), ZELDA_TYPEOF_IS_RVALUE(x)>::type

// #define ZELDA_XTYPEOF_TPL(x) \
// typename zelda::typeof_detail::xtype<typeof_tpl(x), ZELDA_TYPEOF_IS_CONST_TPL(x), ZELDA_TYPEOF_IS_RVALUE_TPL(x)>::type

// }}

// #endif

#endif	/* TYPEOF_H */

