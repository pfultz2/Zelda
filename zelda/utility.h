/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    utility.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_UTILITY_H
#define	ZELDA_GUARD_UTILITY_H

//Thist declares a variable unused by the compiler
//Useful to suppress warnings
#ifdef __GNUC__
#define ZELDA_UNUSED __attribute__((__unused__))
#else
#define ZELDA_UNUSED
#endif

#endif	/* ZELDA_GUARD_UTILITY_H */

