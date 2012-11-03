/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    zlang.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZLANG_GUARD_ZLANG_H
#define ZLANG_GUARD_ZLANG_H


#define Z_ZLANG_CAT_I(x, y) x ## y
#define Z_ZLANG_CAT(x, y) Z_ZLANG_CAT_I(x, y)

#define Z_ZLANG_IS_PAREN(x) Z_ZLANG_IS_PAREN_CHECK(Z_ZLANG_IS_PAREN_PROBE x)
#define Z_ZLANG_IS_PAREN_CHECK(...) Z_ZLANG_IS_PAREN_CHECK_N(__VA_ARGS__,0)
#define Z_ZLANG_IS_PAREN_PROBE(...) ~, 1,
#ifndef _MSC_VER
#define Z_ZLANG_IS_PAREN_CHECK_N(x, n, ...) n
#else
// MSVC workarounds
#define Z_ZLANG_IS_PAREN_CHECK_RES(x) x
#define Z_ZLANG_IS_PAREN_CHECK_II(x, n, ...) n
#define Z_ZLANG_IS_PAREN_CHECK_I(x) Z_ZLANG_IS_PAREN_CHECK_RES(Z_ZLANG_IS_PAREN_CHECK_II x)
#define Z_ZLANG_IS_PAREN_CHECK_N(...) Z_ZLANG_IS_PAREN_CHECK_I((__VA_ARGS__))
#endif

#define Z_ZLANG_REM(m) m, 

#define Z_ZLANG_PRIMITIVE_INVOKE(m, x) m(x)

#define Z_ZLANG_INVOKE(x) Z_ZLANG_PRIMITIVE_INVOKE(Z_ZLANG_REM x)

#define Z_ZLANG_FIND_0(x, raw) Z_ZLANG_CAT(ZLANG_, raw)
#define Z_ZLANG_FIND_1(x, raw) x
#define Z_ZLANG_FIND_I(x, raw) Z_ZLANG_CAT(Z_ZLANG_FIND_, Z_ZLANG_IS_PAREN(x))(x, raw)
#define Z_ZLANG_FIND(x) Z_ZLANG_FIND_I(Z_ZLANG_CAT(ZLANG_, Z_ZLANG_CAT(ZLANG_USE, x)), x)

#define ZLANG(x) Z_ZLANG_INVOKE(Z_ZLANG_FIND(x))

#ifndef ZLANG_DEFAULT_NS
#define ZLANG_DEFAULT_NS
#endif

#ifndef ZLANG_USE
#define ZLANG_USE ZLANG_DEFAULT_NS
#endif
#define $ ZLANG

#endif
