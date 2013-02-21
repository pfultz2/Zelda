/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    test.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ZELDA_TEST_H
#define ZELDA_GUARD_ZELDA_TEST_H

#include <zelda/equals.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <zelda/foreach.h>
#include <vector>
#include <map>

#define ZELDA_TEST_CASE(name) \
struct name \
{ void operator()(zelda::test::failure_callback fail) const; }; \
static zelda::test::auto_register name ## _register = zelda::test::auto_register(#name, name()); \
void name::operator()(zelda::test::failure_callback zelda_test_fail) const

#define ZELDA_DETAIL_TEST_FAIL(message) if (zelda_test_fail(message, __LINE__)) return
#define ZELDA_DETAIL_TEST_FAIL_EX(message) if (zelda_test_fail(message, __LINE__)) throw

#define ZELDA_DETAIL_TEST_EXEC(rethrow, ...) \
do \
{ \
    try { __VA_ARGS__; } \
    catch(const std::exception& zelda_ex) { ZELDA_DETAIL_TEST_FAIL_EX(zelda_ex.what()); rethrow } \
    catch(...) { ZELDA_DETAIL_TEST_FAIL_EX("An unknown exception has occured"); rethrow } \
} while(0)

#define ZELDA_TEST_EXEC(...) ZELDA_DETAIL_TEST_EXEC(throw;, __VA_ARGS__)

#define ZELDA_TEST_EQUAL(x, ...) \
ZELDA_TEST_EXEC(if (!zelda::equals(x, __VA_ARGS__)) ZELDA_DETAIL_TEST_FAIL(std::string(#x) + " != " + #__VA_ARGS__))

namespace zelda { namespace test {

typedef boost::function<bool(std::string message, long line)> failure_callback;
typedef boost::function<void(failure_callback fail)> test_case;
static std::vector<std::pair<std::string, test_case> > test_cases;

struct auto_register
{
    auto_register(std::string name, test_case tc)
    {
        test_cases.push_back(std::make_pair(name, tc));
    }
};

struct out_failure
{
    typedef bool result_type;
    bool operator()(bool & failed, std::string name, std::string message, long line) const
    {
        std::cout << "*****FAILED: " << name << "@" << line << std::endl << message << std::endl;
        failed = true;
        return true;
    }
};

void run()
{
    bool failed = false;
    ZELDA_FOREACH(const auto& tc, test_cases)
    {
        tc.second(boost::bind(out_failure(), boost::ref(failed), tc.first, _1, _2));
    }
    if (!failed) std::cout << "All " << test_cases.size() << " test cases passed." << std::endl;
}

}}

#endif
