/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    result_of.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_RESULT_OF_H
#define ZELDA_GUARD_RESULT_OF_H

#include <zelda/is_callable.h>
#include <zelda/requires.h>

namespace zelda {

template<class F, ZELDA_REQUIRES(is_callable<F>)>
struct result_of
: boost::result_of<F> {};

}



#endif