/* 
 * File:   introspection.h
 * Author: pfultz
 *
 * Created on November 17, 2011, 9:00 PM
 */

#ifndef ZELDA_INTROSPECTION_H
#define	ZELDA_INTROSPECTION_H

#include "pp.h"
#include "requires.h"
#include <boost/mpl/bool.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

/**
 * This macro creates a trait to evaluate if a class has a member function using
 * SFINAE. The trait name will be has_ + <the member name>.
 * This macro takes two parameters:
 * member: this is the name of the member function to check for. This will also
 *      be used to name the type.
 * parameters(optional): This is the list of parameters for the function. The arguments
 *      must be a PP_SEQ. So if the parameters for the function are two ints, it
 *      would be written as (int)(int). If the parameters option is left out, it 
 *      assumed there are no parameters for the function.
 */
#define ZELDA_INTROSPECTION_HAS_MEMBER_FUNCTION template<class this_t> DETAIL_INTROSPECTION_HAS_MEMBER_FUNCTION
#define ZELDA_INTROSPECTION_HAS_MEMBER_FUNCTION_TEMPLATE(...) template<class this_t, __VA_ARGS__> DETAIL_INTROSPECTION_HAS_MEMBER_FUNCTION

#ifndef ZELDA_NO_INTROSPECTION
#define DETAIL_INTROSPECTION_HAS_MEMBER_FUNCTION(...) PP_OPTIONAL_IF(\
	DETAIL_INTROSPECTION_HAS_MEMBER_FUNCTION_SFINAE, \
	DETAIL_INTROSPECTION_HAS_MEMBER_FUNCTION_SFINAE_EMPTY, \
	__VA_ARGS__)
#else
#define DETAIL_INTROSPECTION_HAS_MEMBER_FUNCTION(...) DETAIL_INTROSPECTION_HAS_MEMBER_FUNCTION_NO_INTROSPECTION(PP_OPTIONAL_FIRST(__VA_ARGS__))
#endif


#define DETAIL_INTROSPECTION_VALUE_OF_EACH_ELEM(d, data, ...) value_of< __VA_ARGS__ >()
#define DETAIL_INTROSPECTION_VALUE_OF_EACH_TRANSFORM(args) BOOST_PP_SEQ_TO_TUPLE(BOOST_PP_SEQ_TRANSFORM(DETAIL_INTROSPECTION_VALUE_OF_EACH_ELEM, 0, args))
#define DETAIL_INTROSPECTION_VALUE_OF_EACH(args) BOOST_PP_IF(BOOST_PP_SEQ_SIZE(args),  \
DETAIL_INTROSPECTION_VALUE_OF_EACH_TRANSFORM, \
PP_NOP \
)(args)


#define DETAIL_INTROSPECTION_HAS_MEMBER_FUNCTION_SFINAE(member, args) \
struct BOOST_PP_CAT(has_, member) \
{ \
    template<class U> \
    static U value_of(); \
    typedef char yes; \
    typedef long no; \
    template<class N> struct selector; \
\
    template<class U> \
    static yes check( selector<typeof( value_of<U>().member( DETAIL_INTROSPECTION_VALUE_OF_EACH(args)) )> * ); \
\
    template<class U> \
    static no check(...); \
    \
    static const bool value = sizeof(check<this_t>(0)) == sizeof(yes); \
\
    typedef boost::mpl::bool_<value> type;\
};\

#define DETAIL_INTROSPECTION_HAS_MEMBER_FUNCTION_SFINAE_EMPTY(member) \
struct BOOST_PP_CAT(has_, member) \
{ \
    template<class U> \
    static U value_of(); \
    typedef char yes; \
    typedef long no; \
    template<class N> struct selector; \
\
    template<class U> \
    static yes check( selector<typeof( value_of<U>().member() )> * ); \
\
    template<class U> \
    static no check(...); \
    \
    static const bool value = sizeof(check<this_t>(0)) == sizeof(yes); \
\
    typedef boost::mpl::bool_<value> type;\
};\

#define DETAIL_INTROSPECTION_HAS_MEMBER_FUNCTION_NO_INTROSPECTION(member) \
struct BOOST_PP_CAT(has_, member) \
{ \
    static const bool value = false; \
    typedef boost::mpl::bool_<value> type;\
};\


#ifndef ZELDA_NO_INTROSPECTION
namespace test_detail
{
ZELDA_INTROSPECTION_HAS_MEMBER_FUNCTION(foo);
struct test_true
{
    void foo();
};
struct test_false
{
};
BOOST_STATIC_ASSERT((has_foo<test_true>::value == true));
BOOST_STATIC_ASSERT((has_foo<test_false>::value == false));
}
#endif

#endif	/* ZELDA_INTROSPECTION_H */

