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


class TestFactory : public TestCase {
public:
    void run() override {
        class TestProduct {
        public:
            int value;
            TestProduct(int v) : value(v) {}
        };
        
        Factory<int, TestProduct> factory;
        
        factory.registerCreator(5, []() { return std::make_unique<TestProduct>(10); });
        factory.registerCreator(10, []() { return std::make_unique<TestProduct>(20); });
        
        auto product1 = factory.create(5);
        auto product2 = factory.create(10);
        
        assert(product1);
        assert(product2);
        assert(product1->value == 10);
        assert(product2->value == 20);
        assert(product1 != product2);
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
        
        assert(flyweight1);
        assert(flyweight2);
        assert(flyweight3);
        assert(flyweight1 == flyweight2);
        assert(flyweight1 != flyweight3);
        assert(flyweight1->data == "flyweight_test1");
        assert(flyweight3->data == "flyweight_test2");
    }
};


int main() {
    TestRunner runner;
    runner.addTest("Utils: Chain of Responsibility", std::make_shared<TestChain>());
    runner.addTest("Utils: Singleton", std::make_shared<TestSingleton>());
    runner.addTest("Utils: Factory", std::make_shared<TestFactory>());
    runner.addTest("Utils: Flyweight Factory", std::make_shared<TestFlyweightFactory>());
    runner.runAll();

    return 0;
}

#endif // isTEST