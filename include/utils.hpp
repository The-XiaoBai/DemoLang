/**
 * @file include/utils.hpp
 * @brief Utilities for DemoLang.
 * @author The-XiaoBai
 * @date 2026/01/31
**/

#pragma once
#ifndef DEMOLANG_UTILS
#define DEMOLANG_UTILS

#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>
#include <stdexcept>
#include <string>


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
 * @brief Simplified factory pattern template using creator-based registration only
 * @tparam KeyType The key type for registration (e.g., std::string)
 * @tparam ProductType The product type to create
 * @tparam CreatorType The creator function type (default: std::function<std::unique_ptr<ProductType>()>)
 * @tparam ReturnType The return type for execute method (void by default)
 * @tparam ArgTypes The argument types for execute method
 */
template <typename KeyType, typename ProductType, 
          typename CreatorType = std::function<std::unique_ptr<ProductType>()>,
          typename ReturnType = void, typename... ArgTypes>
class Factory {
private:
    std::unordered_map<KeyType, CreatorType> creators;
    bool initialized = false;

public:
    /**
     * @brief Register a product creator with a key
     * @param key The registration key
     * @param creator The creator function
     */
    void registerCreator(const KeyType& key, CreatorType creator) {
        creators[key] = std::move(creator);
    }

    /**
     * @brief Create a product by key using registered creator
     * @param key The registration key
     * @return Unique pointer to the created product
     */
    std::unique_ptr<ProductType> create(const KeyType& key) {
        auto it = creators.find(key);
        if (it != creators.end()) {
            return it->second();
        }
        return nullptr;
    }

    /**
     * @brief Execute a product by key using creator (requires ProductType to have execute method)
     * @param key The registration key
     * @param args The arguments to pass to execute method
     * @return The result of execute method
     */
    ReturnType execute(const KeyType& key, ArgTypes... args) {
        auto it = creators.find(key);
        if (it != creators.end()) {
            auto product = it->second();
            if constexpr (std::is_void_v<ReturnType>) {
                product->execute(std::forward<ArgTypes>(args)...);
            } else {
                return product->execute(std::forward<ArgTypes>(args)...);
            }
        }
        if constexpr (!std::is_void_v<ReturnType>) {
            throw std::runtime_error("Product not found");
        }
    }

    /**
     * @brief Check if a key is registered
     * @param key The registration key
     * @return True if registered, false otherwise
     */
    bool isRegistered(const KeyType& key) const {
        return creators.find(key) != creators.end();
    }

    /**
     * @brief Get all registered keys
     * @return Vector of registered keys
     */
    std::vector<KeyType> getRegisteredKeys() const {
        std::vector<KeyType> keys;
        for (const auto& pair : creators) {
            keys.push_back(pair.first);
        }
        return keys;
    }

    /**
     * @brief Initialize the factory (call once before use)
     * @tparam InitFunc The initialization function type
     * @param initFunc The initialization function
     */
    template <typename InitFunc>
    void initialize(InitFunc initFunc) {
        if (!initialized) {
            initFunc(*this);
            initialized = true;
        }
    }

    /**
     * @brief Check if factory is initialized
     * @return True if initialized, false otherwise
     */
    bool isInitialized() const {
        return initialized;
    }

    /**
     * @brief Reset the factory (clear all registrations)
     */
    void reset() {
        creators.clear();
        initialized = false;
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


} // namespace Utils

} // namespace DemoLang

#endif // DEMOLANG_UTILS