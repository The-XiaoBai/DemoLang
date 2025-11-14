#pragma once
#ifndef TEST_FRAMEWORK
#define TEST_FRAMEWORK

#include <cassert>
#include <memory>
#include <vector>
#include <iostream>


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

public:
    void addTest(const std::string& name, std::shared_ptr<TestCase> test) {
        tests.emplace_back(name, test);
    }

    void runAll() {
        int passed = 0;
        for (const auto& [name, test] : tests) {
            try {
                test->setUp();
                test->run();
                test->tearDown();
                std::cout << "[PASS] " << name << std::endl;
                passed++;
            } catch (const std::exception& e) {
                std::cerr << "[FAIL] " << name << ": " << e.what() << std::endl;
                test->tearDown();
            }
        }
        std::cout << "Tests passed: " << passed << "/" << tests.size() << std::endl;
    }
};

#endif // TEST_FRAMEWORK