/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    test.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ZELDA_TEST_H
#define ZELDA_GUARD_ZELDA_TEST_H

#include <boost/function.hpp>
#include <vector>
#include <map>

#define ZELDA_TEST_CASE(name) \
struct name \
{ void operator()(failure_callback fail); }; \
static zelda::test::auto_register name ## _register = auto_register(#name, name()); \
void name::operator()(failure_callback zelda_test_fail)

#define ZELDA_DETAIL_TEST_FAIL(message) if (zelda_test_fail(message, __LINE__)) return
#define ZELDA_DETAIL_TEST_FAIL_EX(message) if (zelda_test_fail(message, __LINE__)) throw

#define ZELDA_DETAIL_TEST_EXEC(rethrow, ...) \
do \
{ \
    try { __VA_ARGS__ } \
    catch(const std::exception& zelda_ex) { ZELDA_DETAIL_TEST_FAIL_EX(ex.what()); rethrow } \
    catch(...) { ZELDA_DETAIL_TEST_FAIL_EX("An unknown exception has occured"); rethrow } \
} while(0)

#define ZELDA_TEST_EXEC(...) ZELDA_DETAIL_TEST_EXEC(throw;, __VA_ARGS__)

#define ZELDA_TEST_EQUAL(x, ...) \
ZELDA_TEST_EXEC(if (x != __VA_ARGS__) ZELDA_DETAIL_TEST_FAIL(std::string(#x) + " != " + #__VA_ARGS__))

namespace zelda { namespace test {

template<class T>
std::string debug_string(const T & x);

template<class T, class U>
bool debug_equal(const T& x, const T& y);

typedef boost::function<bool(std::string message, long line)> failure_callback;
typedef boost::function<void(failure_callback fail)> test_case;
static std::vector<std::pair<std::string, test_case> test_cases;

struct auto_register
{
    auto_register(std::string name, test_case tc)
    {
        test_cases.push_back(std::make_pair(name, tc));
    }
};

}}

#endif
