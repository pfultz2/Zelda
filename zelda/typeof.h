/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    typeof.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_TYPEOF_H
#define	ZELDA_GUARD_TYPEOF_H

#include <zelda/static_assert.h>
#include <boost/type_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>

#if !defined(ZELDA_NO_DECLTYPE)
#define ZELDA_TYPEOF decltype
#define ZELDA_TYPEOF_TPL decltype
#elif defined(ZELDA_HAS_TYPEOF)
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
    #if !defined(ZELDA_NO_NOEXCEPT) && !defined(ZELDA_NO_RVALUE_REFS)
    #define ZELDA_RETURNS(...) \
        noexcept(noexcept(decltype(__VA_ARGS__)(zelda::typeof_detail::move(ZELDA_AVOID(__VA_ARGS__))))) \
        -> decltype(__VA_ARGS__) \
        { return (__VA_ARGS__); } static_assert(true, "")
    #else
    #define ZELDA_RETURNS(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; } static_assert(true, "")
    #endif
#else
#define ZELDA_RETURNS(...) ZELDA_ERROR_TRAILING_RETURNS_NOT_SUPPORTED
#endif


#ifndef ZELDA_NO_DECLTYPE
#define ZELDA_XTYPEOF(...) decltype((__VA_ARGS__))
#define ZELDA_XTYPEOF_TPL(...) decltype((__VA_ARGS__))
#else
#define ZELDA_XTYPEOF(...) zelda::typeof_detail::xtypeof_<ZELDA_TYPEOF(__VA_ARGS__), ZELDA_TYPEOF_IS_LVALUE(__VA_ARGS__), ZELDA_TYPEOF_IS_RVALUE(__VA_ARGS__)>::type
#define ZELDA_XTYPEOF_TPL(...) typename zelda::typeof_detail::xtypeof_<ZELDA_TYPEOF_TPL(__VA_ARGS__), ZELDA_TYPEOF_IS_LVALUE_TPL(__VA_ARGS__), ZELDA_TYPEOF_IS_RVALUE_TPL(__VA_ARGS__)>::type
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
#ifndef ZELDA_NO_RVALUE_REFS
template<class T> T&& operator,(T&&, void_);
#else
template<class T> const T& operator,(T const&, void_);
template<class T> T& operator,(T&, void_);
#endif
#define ZELDA_AVOID(...) ((__VA_ARGS__), zelda::typeof_detail::void_())

#ifndef ZELDA_NO_RVALUE_REFS
template<typename T>
typename std::remove_reference<T>::type&&
move(T&& x) noexcept;
void move(void_);
#endif

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
    operator const T&() const;
};

template<typename T>
rvalue_probe<T> const make_probe(T const &);
void const make_probe(void_);

#define ZELDA_TYPEOF_RVALUE_PROBE(x) true ? zelda::typeof_detail::make_probe(ZELDA_AVOID(x)) : (x)

#define ZELDA_TYPEOF_IS_RVALUE(x) \
boost::mpl::and_<boost::mpl::not_<boost::is_array<ZELDA_TYPEOF(x)> >,\
boost::mpl::not_<boost::is_const<ZELDA_TYPEOF(ZELDA_TYPEOF_RVALUE_PROBE(x))> > >

#define ZELDA_TYPEOF_IS_RVALUE_TPL(x) \
boost::mpl::and_<boost::mpl::not_<boost::is_array<ZELDA_TYPEOF_TPL(x)> >,\
boost::mpl::not_<boost::is_const<ZELDA_TYPEOF_TPL(ZELDA_TYPEOF_RVALUE_PROBE(x))> > >

template<class T> boost::mpl::false_ is_lvalue(const T &);
template<class T> boost::mpl::true_ is_lvalue(T&);
inline boost::mpl::false_ is_lvalue(void_);

#define ZELDA_TYPEOF_IS_LVALUE(x) \
ZELDA_TYPEOF(zelda::typeof_detail::is_lvalue(ZELDA_AVOID(x)))

#define ZELDA_TYPEOF_IS_LVALUE_TPL(x) \
ZELDA_TYPEOF_TPL(zelda::typeof_detail::is_lvalue(ZELDA_AVOID(x)))

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
namespace zelda {
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
    typedef ZELDA_XTYPEOF_TPL(T::by_value()) test2;

    //On MSVC, static_assert is broken, so we use BOOST_MPL_ASSERT instead.
    BOOST_MPL_ASSERT_NOT((boost::is_reference<ZELDA_XTYPEOF_TPL(T::by_value())>));
    BOOST_MPL_ASSERT_NOT((zelda::typeof_detail::is_const2<ZELDA_XTYPEOF_TPL(T::by_value())>));

    BOOST_MPL_ASSERT((boost::is_reference<ZELDA_XTYPEOF_TPL(T::by_ref())>));
    BOOST_MPL_ASSERT_NOT((zelda::typeof_detail::is_const2<ZELDA_XTYPEOF_TPL(T::by_ref())>));

    BOOST_MPL_ASSERT((boost::is_reference<ZELDA_XTYPEOF_TPL(T::by_const_ref())>));
    BOOST_MPL_ASSERT((zelda::typeof_detail::is_const2<ZELDA_XTYPEOF_TPL(T::by_const_ref())>));

    BOOST_MPL_ASSERT((boost::is_same<ZELDA_XTYPEOF_TPL(T::by_void()), void>));
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


#endif	/* TYPEOF_H */

