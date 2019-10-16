//
// Created by sneeuwpop on 01-10-19.
//

#ifndef SNEEUWPOP_UNIT_TESTING_H
#define SNEEUWPOP_UNIT_TESTING_H

#include <functional>
#include <iostream>
#include <vector>
#include <cstring>

#undef NDEBUG
#include "assert.h"

/**
 * I tried to get GTest working nicely but that did not work like I wanted.
 * After that I tried CxxTest and Boost.Test but they where to bloated.
 * As a final attempt I tried Catch2 witch was sort of nice.
 *
 * So The final result was that I implemented my own unit testing.
 * This is a dead simple implementation of the auto registering factory pattern.
 * http://derydoca.com/2019/03/c-tutorial-auto-registering-factory/#
 *
 *                        ___HOW_IT_WORKS___
 *  1. Every TEST macro gets converted to a class.
 *  2. Then a global variable of the same type as this class is created,
 *  3. This then directly calls the constructor before the main gets called
 *  4. The constructor registers itself at this header only library
 *  5. Add the macro RUN_ALL_TEST() to your main and all tests will be fired.
 *
 */

#define ASSERT_EQ(a, b)  if (a != b) {  std::cerr << a << " != " << b << "\n"; return false; }
#define ASSERT_NOT_EQ(a, b)  if (a == b) {  std::cerr << a << " == " << b << "\n"; return false; }

#define RUN_ALL_TESTS() UnitTest::instance().runAllTests();
#define TEST(NAME, CODE) \
    class Tmp##NAME { \
    public: \
        inline Tmp##NAME() { UnitTest::instance().registeredFunctions.emplace_back(TestCase{#NAME, [&] { CODE return true; }, __FILE__, __LINE__}); } \
    };\
    inline Tmp##NAME test##NAME;\


// https://stackoverflow.com/a/2616912
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_RESET "\033[0m"



struct TestCase {
    const char *name;
    std::function<bool()> validateSucces;
    const char *file;
    const unsigned int line;
};

class UnitTest {
private:
    UnitTest() = default;

    int successfulTests = 0;

    void printResult()
    {
        if (registeredFunctions.empty()) {
            std::cerr << COLOR_YELLOW << "============================================" << "\n";
            std::cerr << "\tYou have no test in your code" << "\n\n" << COLOR_RESET;
        } else if (successfulTests == registeredFunctions.size()) {
            std::cerr << COLOR_GREEN << "============================================" << "\n";
            std::cerr << "\t All tests where successful (" << successfulTests <<"/"<< successfulTests <<")\n\n" << COLOR_RESET;
        } else {
            std::cerr << COLOR_RED << "============================================" << "\n";
            std::cerr << "\t Not all tests where successful (" << successfulTests <<"/"<< registeredFunctions.size() <<")\n\n" << COLOR_RESET;
        }
    }

public:
    static UnitTest& instance()  // singleton pattern
    {
        static UnitTest INSTANCE;
        return INSTANCE;
    }

    std::vector<TestCase> registeredFunctions = {};

    void runAllTests()
    {
        for (auto testCase : registeredFunctions) {
            if (testCase.validateSucces()) {
                successfulTests++;
            } else {
                std::cerr << COLOR_RED;
                std::cerr << "__UNIT_TEST_ERROR__ \n\n";
                std::cerr << "ERROR IN UNIT TEST : " << testCase.name << "\n";
                std::cerr << "FOUND AT : " << testCase.file << ":" << testCase.line <<  "\n\n";
                std::cerr << COLOR_RESET;
                break;
            }
        }
        printResult();
    }
};


#endif