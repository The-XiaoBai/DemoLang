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
            ValueHandler() : Handler() {}
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

        assert(result);
        assert(*result == 3);
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
        
        assert(&instance1 == &instance2);
        assert(instance1.value == 0);
        assert(instance2.value == 0);
        
        instance1.value = 42;
        assert(instance1.value == 42);
        assert(instance2.value == 42);
    }
};


int main() {
    TestRunner runner;
    runner.addTest("Utils: Chain of Responsibility", std::make_shared<TestChain>());
    runner.addTest("Utils: Singleton", std::make_shared<TestSingleton>());
    runner.runAll();

    return 0;
}

#endif // isTEST