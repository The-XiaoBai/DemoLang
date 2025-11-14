/**
 * @file include/utils.hpp
 * @brief Utilities for DemoLang.
 * @author The-XiaoBai
 * @date 2025/11/14
**/

#pragma once
#ifndef DEMOLANG_UTILS
#define DEMOLANG_UTILS

#include <memory>


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
    };
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
    };
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

    static T* instancePtr;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

public:
    static T& instance() {
        static T instance;
        return instance;
    }
};
template <typename T>
T* Singleton<T>::instancePtr = nullptr;

} // namespace Utils

} // namespace DemoLang

#endif // DEMOLANG_UTILS