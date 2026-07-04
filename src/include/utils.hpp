/**
 * @file src/include/utils.hpp
 * @brief Utilities for DemoLang.
**/

#pragma once
#ifndef DEMOLANG_UTILS
#define DEMOLANG_UTILS

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace DemoLang {

namespace Utils {

/**
 * @brief Base class for a handler in a chain of handlers.
 * @tparam T The data type to be processed.
**/
template <typename T>
class Handler {
protected:
    std::shared_ptr<Handler<T>> nextHandler;

public:
    virtual ~Handler() = default;
    void setNext(std::shared_ptr<Handler<T>> next) {
        nextHandler = next;
    }
    virtual std::shared_ptr<T> handle() = 0;
};

/**
 * @brief Chain of handlers.
 * @tparam T The data type to be processed.
**/
template <typename T>
class Chain {
private:
    std::shared_ptr<Handler<T>> head;
    std::shared_ptr<Handler<T>> tail;

public:
    void addHandler(std::shared_ptr<Handler<T>> handler) {
        if (!head) {
            head = tail = handler;
        } else {
            tail->setNext(handler);
            tail = handler;
        }
    }
    std::shared_ptr<T> execute() {
        return head ? head->handle() : nullptr;
    }
};

/**
 * @brief Singleton class template
 * @tparam T singleton class type
 */
template <typename T>
class Singleton {
private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

public:
    static T& instance() {
        static T instance;
        return instance;
    }
};

/**
 * @brief Flyweight pattern implementation for managing shared objects
 * @tparam KeyType The key type for flyweight lookup
 * @tparam ObjectType The object type to be shared
 * @tparam HashType The hash function type (defaults to std::hash<KeyType>)
 */
template <typename KeyType, typename ObjectType, typename HashType = std::hash<KeyType>>
class FlyweightFactory : public Singleton<FlyweightFactory<KeyType, ObjectType, HashType>> {
private:
    std::unordered_map<KeyType, std::shared_ptr<ObjectType>, HashType> pool;
    
    FlyweightFactory() = default;
    friend class Singleton<FlyweightFactory<KeyType, ObjectType, HashType>>;
    
public:
    std::shared_ptr<ObjectType> getFlyweight(const KeyType& key, std::function<std::shared_ptr<ObjectType>()> creator) {
        auto it = pool.find(key);
        if (it != pool.end()) {
            return it->second;
        }
        
        auto obj = creator();
        pool[key] = obj;
        return obj;
    }
    
    void clear() { pool.clear(); }
    size_t size() const { return pool.size(); }
};

/**
 * @brief Registry pattern: key → function dispatcher (singleton)
 * @tparam KeyType The key type (e.g., std::string)
 * @tparam ReturnType Return type of registered functions
 * @tparam ArgTypes Argument types for registered functions
 */
template <typename KeyType, typename ReturnType, typename... ArgTypes>
class Registry : public Singleton<Registry<KeyType, ReturnType, ArgTypes...>> {
private:
    std::unordered_map<KeyType, std::function<ReturnType(ArgTypes...)>> funcs;

    Registry() = default;
    friend class Singleton<Registry<KeyType, ReturnType, ArgTypes...>>;

public:
    void registerFunc(const KeyType& key, std::function<ReturnType(ArgTypes...)> func) {
        funcs[key] = std::move(func);
    }

    ReturnType execute(const KeyType& key, ArgTypes... args) {
        auto it = funcs.find(key);
        if (it != funcs.end()) {
            return it->second(std::forward<ArgTypes>(args)...);
        }
        throw std::runtime_error("Key not found in registry");
    }

    bool isRegistered(const KeyType& key) const {
        return funcs.find(key) != funcs.end();
    }

    void clear() { funcs.clear(); }
    size_t size() const { return funcs.size(); }
};

} // namespace Utils

} // namespace DemoLang

#endif // DEMOLANG_UTILS