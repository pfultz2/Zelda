/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    fuse.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_FUSE_H
#define ZELDA_GUARD_FUNCTION_FUSE_H

#include <zelda/function/adaptor.h>

namespace zelda { 

//fuse
struct fuse_adaptor_base
{
    ZELDA_AUTO_DECLARE_RESULT()

    ZELDA_AUTO_RESULT((self, f, seq)(zelda::invoke(f, seq)))
};
ZELDA_FUNCTION_MAKE_SIMPLE_ADAPTOR(fuse)

}

#endif
