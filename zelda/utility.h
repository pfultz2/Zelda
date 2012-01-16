/* 
 * File:   utility.h
 * Author: pfultz
 *
 * Created on November 17, 2011, 8:45 PM
 */

#ifndef UTILITY_H
#define	UTILITY_H

//Thist declares a variable unused by the compiler
//Useful to suppress warnings
#ifdef __GNUC__
#define ZELDA_UNUSED __attribute__((__unused__))
#else
#define ZELDA_UNUSED
#endif

#endif	/* UTILITY_H */

