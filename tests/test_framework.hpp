/**
 * @file tests/test_framework.hpp
 * @brief Lightweight test framework for unit tests.
 **/

#pragma once
#ifndef TEST_FRAMEWORK
#define TEST_FRAMEWORK

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            throw std::runtime_error("\t" + std::string(__FILE__) + ":" + \
                std::to_string(__LINE__) + "\n\t" + #condition); \
        } \
    } while (0)

class TestCase {
public:
    virtual ~TestCase() = default;
    virtual void setUp() {}
    virtual void tearDown() {}
    virtual void run() = 0;
};

class TestRunner {
private:
    std::vector<std::pair<std::string, std::shared_ptr<TestCase>>> tests;
    int failures = 0;
    int passed = 0;

public:
    void addTest(const std::string& name, std::shared_ptr<TestCase> test) {
        tests.emplace_back(name, test);
    }

    /**
     * @brief Run all registered tests.
     * @return int Exit code: 0 if all tests pass, 1 if any test fails.
     **/
    int runAll() {
        for (const auto& [name, test] : tests) {
            try {
                test->setUp();
                test->run();
                test->tearDown();
                std::cout << "[PASS] " << name << std::endl;
                passed++;
            } catch (const std::exception& e) {
                test->tearDown();
                std::cerr << "[FAIL] " << name << "\n" << e.what() << std::endl;
                failures++;
            } catch (...) {
                test->tearDown();
                std::cerr << "[FAIL] " << name << "\n\tUnknown exception" << std::endl;
                failures++;
            }
        }
        std::cout << "Tests passed: " << passed << "/" << tests.size() << std::endl;
        std::cout << std::endl << std::endl;
        return failures > 0 ? 1 : 0;
    }
};

#endif // TEST_FRAMEWORK