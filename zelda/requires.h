/* 
 * File:   requires.h
 * Author: pfultz
 *
 * Created on November 17, 2011, 8:47 PM
 */

#ifndef ZELDA_REQUIRES_H
#define	ZELDA_REQUIRES_H

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/logical.hpp>

/**
 * The ZELDA_FUNCTION_REQUIRES macro is used to place conditions on template 
 * functions. NOTE: If an error occurs here it is because you are most likely
 * missing the return type, or the return type is missing parenthesis around it.
 * 
 * The macro is used in this way:
 *      ZELDA_FUNCTION_REQUIRES(Cond)(ReturnType)
 * Cond: Is the condition the function requires
 * ReturnType: Is the return type of the function
 * 
 * Here is an example of a sum function, that sums two numbers and requires the 
 * type to be numeric:
 *      template<class T>
 *      ZELDA_FUNCTION_REQUIRES(is_numeric<T>)(T)
 *      sum(T x, T y) { return x+y; }
 * 
 * 
 */
#define ZELDA_ERROR_PARENTHESIS_MUST_BE_PLACED_AROUND_THE_RETURN_TYPE(...) __VA_ARGS__>::type
#define ZELDA_FUNCTION_REQUIRES(...) typename boost::enable_if<boost::mpl::and_<__VA_ARGS__, boost::mpl::bool_<true> >, ZELDA_ERROR_PARENTHESIS_MUST_BE_PLACED_AROUND_THE_RETURN_TYPE
#define ZELDA_EXCLUDE(...) typename boost::mpl::not_<typename boost::mpl::or_<__VA_ARGS__, boost::mpl::bool_<false> >::type >::type

#define ZELDA_CLASS_REQUIRES(...) typename boost::enable_if<boost::mpl::and_<__VA_ARGS__, boost::mpl::bool_<true> > >::type

#endif	/* ZELDA_REQUIRES_H */

