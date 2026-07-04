/**
 * @file tests/test_utils.cpp
 * @brief Unit tests for the utils module.
 **/

#ifdef isTEST

#include "test_framework.hpp"
#include "utils.hpp"

using namespace DemoLang;
using namespace DemoLang::Utils;

class TestChain : public TestCase {
public:
    void run() override {
        static int value;
        class ValueHandler : public Handler<int> {
        public:
            ValueHandler() = default;
            std::shared_ptr<int> handle() override {
                value++;
                if (!nextHandler) return std::make_shared<int>(value);
                return nextHandler->handle();
            }
        };
        Chain<int> chain;
        chain.addHandler(std::make_shared<ValueHandler>());
        chain.addHandler(std::make_shared<ValueHandler>());
        chain.addHandler(std::make_shared<ValueHandler>());
        auto result = chain.execute();

        ASSERT(result);
        ASSERT(*result == 3);
    }
};

class TestSingleton : public TestCase {
public:
    void run() override {
        class TestClass : public Singleton<TestClass> {
        public:
            int value;
            TestClass() : value(0) {}
        };
        
        auto& instance1 = TestClass::instance();
        auto& instance2 = TestClass::instance();
        
        ASSERT(&instance1 == &instance2);
        ASSERT(instance1.value == 0);
        ASSERT(instance2.value == 0);
        
        instance1.value = 42;
        ASSERT(instance1.value == 42);
        ASSERT(instance2.value == 42);
    }
};

class TestRegistry : public TestCase {
public:
    void run() override {
        class TestRegistryClass : public Singleton<TestRegistryClass> {
        private:
            std::unordered_map<int, std::function<int(int, int)>> funcs;
            TestRegistryClass() = default;
            friend class Singleton<TestRegistryClass>;
        public:
            void registerFunc(int key, std::function<int(int, int)> func) {
                funcs[key] = std::move(func);
            }
            int execute(int key, int a, int b) {
                return funcs[key](a, b);
            }
        };

        auto& reg = TestRegistryClass::instance();
        reg.registerFunc(0, [](int a, int b) { return a + b; });
        reg.registerFunc(1, [](int a, int b) { return a - b; });

        ASSERT(reg.execute(0, 5, 3) == 8);
        ASSERT(reg.execute(1, 5, 3) == 2);
    }
};

class TestFlyweightFactory : public TestCase {
public:
    void run() override {
        class TestFlyweight {
        public:
            std::string data;
            TestFlyweight(const std::string& d) : data(d) {}
        };
        
        auto& factory = FlyweightFactory<std::string, TestFlyweight>::instance();
        
        factory.clear();
        
        auto flyweight1 = factory.getFlyweight("test1", []() { 
            return std::make_shared<TestFlyweight>("flyweight_test1"); 
        });
        auto flyweight2 = factory.getFlyweight("test1", []() { 
            return std::make_shared<TestFlyweight>("flyweight_test1"); 
        });
        auto flyweight3 = factory.getFlyweight("test2", []() { 
            return std::make_shared<TestFlyweight>("flyweight_test2"); 
        });
        
        ASSERT(flyweight1);
        ASSERT(flyweight2);
        ASSERT(flyweight3);
        ASSERT(flyweight1 == flyweight2);
        ASSERT(flyweight1 != flyweight3);
        ASSERT(flyweight1->data == "flyweight_test1");
        ASSERT(flyweight3->data == "flyweight_test2");
    }
};

int main() {
    TestRunner runner;
    runner.addTest("Utils: Chain of Responsibility", std::make_shared<TestChain>());
    runner.addTest("Utils: Singleton", std::make_shared<TestSingleton>());
    runner.addTest("Utils: Registry", std::make_shared<TestRegistry>());
    runner.addTest("Utils: Flyweight Factory", std::make_shared<TestFlyweightFactory>());
    return runner.runAll();
}

#endif // isTEST