/* 
 * File:   typeof.h
 * Author: paul
 *
 * Created on January 12, 2012, 9:29 PM
 */

#ifndef TYPEOF_H
#define	TYPEOF_H

#include <boost/typeof/typeof.hpp>
#include <boost/type_traits.hpp>

#include "mpl.h"

#define ZELDA_TYPEOF decltype

namespace zelda {
template<class T>
T&& declval();

namespace typeof_detail {
struct value {};
struct rvalue {};
struct const_lvalue {};
struct lvalue {};

//is_const2 that works with references
template<class T>
struct is_const2 : boost::is_const<typename boost::remove_reference<T>::type >
{
};

template<class T>
typename zelda::mpl::if_<std::is_rvalue_reference<T&&>, rvalue>
                                ::template else_if<is_const2<T&&>, const_lvalue, lvalue >
                            ::type test(T&&);

template<class Tag, class T>
struct xtypeof {};

template<class T>
struct xtypeof<rvalue, T> : std::add_rvalue_reference<T> {};

template<class T>
struct xtypeof<lvalue, T> : std::add_lvalue_reference<T> {};

template<class T>
struct xtypeof<const_lvalue, T> : std::add_const< typename std::add_lvalue_reference<T>::type > {};

template<class T>
struct xtypeof<value, T> : boost::mpl::identity<T> {};
}
}

#define ZELDA_XTYPEOF(...) \
typename zelda::typeof_detail::xtypeof<ZELDA_TYPEOF(zelda::typeof_detail::test(__VA_ARGS__)), ZELDA_TYPEOF(__VA_ARGS__)>::type

#define ZELDA_RETURNS(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

#ifdef ZELDA_TEST
namespace zelda {
namespace typeof_test {

int by_value();
int&& by_rvalue();
int& by_ref();
const int& by_const_ref();



}
}

BOOST_STATIC_ASSERT((std::is_rvalue_reference<ZELDA_XTYPEOF(zelda::typeof_test::by_value())>::value));
BOOST_STATIC_ASSERT((not zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(zelda::typeof_test::by_value())>::value));

BOOST_STATIC_ASSERT((std::is_rvalue_reference<ZELDA_XTYPEOF(zelda::typeof_test::by_rvalue())>::value));
BOOST_STATIC_ASSERT((not zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(zelda::typeof_test::by_rvalue())>::value));

BOOST_STATIC_ASSERT((std::is_lvalue_reference<ZELDA_XTYPEOF(zelda::typeof_test::by_ref())>::value));
BOOST_STATIC_ASSERT((not zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(zelda::typeof_test::by_ref())>::value));

BOOST_STATIC_ASSERT((std::is_lvalue_reference<ZELDA_XTYPEOF(zelda::typeof_test::by_const_ref())>::value));
BOOST_STATIC_ASSERT((zelda::typeof_detail::is_const2<ZELDA_XTYPEOF(zelda::typeof_test::by_const_ref())>::value));

#endif

#if 0

#ifdef NETBEANS
#define typeof decltype
#endif

#ifdef ZELDA_HAS_DECLTYPE
#define typeof decltype
#define typeof_tpl decltype
#endif

#ifndef ZELDA_HAS_DECLTYPE

#if defined(__GNUC__) || defined(__clang__)
#define typeof_tpl typeof

#elif defined(_MSC_VER)
#if _MSC_VER >= 1600
#define typeof decltype
#define typeof_tpl decltype
#else
#define typeof BOOST_TYPEOF
#define typeof_tpl BOOST_TYPEOF_TPL
#endif

#else
#define typeof BOOST_TYPEOF
#define typeof_tpl BOOST_TYPEOF_TPL
#endif

#define ZELDA_TYPEOF typeof

#endif

namespace zelda {

template<class T>
T declval();


namespace typeof_detail {

template<class T>
struct identity
{
    typedef T type;
};

template<class T>
struct wrap
{
    typedef typename T::type type;
};

template<class T>
boost::mpl::true_ is_const(const T& x);

template<class T>
boost::mpl::false_ is_const(T & x);

template<class T>
inline typename boost::is_array<T>::type is_array(T const &);

template<class T>
inline boost::mpl::false_ is_rvalue(T &, int);

template<class T>
inline boost::mpl::true_ is_rvalue(T const &, ...);

//rvalue probe from Eric Niebler
template<typename T>
struct rvalue_probe
{
    struct private_type_ {};
    // can't ever return an array by value
    typedef BOOST_DEDUCED_TYPENAME boost::mpl::if_<
        boost::mpl::or_<boost::is_abstract<T>, boost::is_array<T> >, private_type_, T
    >::type value_type;
    operator value_type() { return *reinterpret_cast<value_type *>(this); } // never called
    operator T &() const { return *reinterpret_cast<T *>(const_cast<rvalue_probe *>(this)); } // never called
};

template<typename T>
rvalue_probe<T> const make_probe(T const &)
{
    return rvalue_probe<T>();
}

template<class Cond, class T>
struct add_const_if : boost::mpl::if_<Cond, typename boost::add_const<T>::type, T >
{
};

template<class IsConst, class T>
struct xtype_ref : add_const_if<IsConst, typename boost::add_reference<T>::type >
{   
};

template<class T, class IsConst, class IsRvalue>
struct xtype : boost::mpl::if_<IsRvalue, T, typename xtype_ref<IsConst, T>::type>
{
};

#define ZELDA_TYPEOF_IS_RVALUE(x) \
boost::mpl::and_<boost::mpl::not_<typeof(zelda::typeof_detail::is_array(x))>,\
typeof(zelda::typeof_detail::is_rvalue(true ? zelda::typeof_detail::make_probe(x) : (x), 0))>

#define ZELDA_TYPEOF_IS_RVALUE_TPL(x) \
boost::mpl::and_<boost::mpl::not_<typeof_tpl(zelda::typeof_detail::is_array(x))>,\
typeof_tpl(zelda::typeof_detail::is_rvalue(true ? zelda::typeof_detail::make_probe(x) : (x), 0))>

#define ZELDA_TYPEOF_IS_CONST(x) \
typeof(zelda::typeof_detail::is_const(x))

#define ZELDA_TYPEOF_IS_CONST_TPL(x) \
typeof_tpl(zelda::typeof_detail::is_const(x))

#define ZELDA_XTYPEOF(x) \
zelda::typeof_detail::xtype<typeof(x), ZELDA_TYPEOF_IS_CONST(x), ZELDA_TYPEOF_IS_RVALUE(x)>::type

#define ZELDA_XTYPEOF_TPL(x) \
typename zelda::typeof_detail::xtype<typeof_tpl(x), ZELDA_TYPEOF_IS_CONST_TPL(x), ZELDA_TYPEOF_IS_RVALUE_TPL(x)>::type

}}

#endif

#endif	/* TYPEOF_H */

