/**
 * @file src/include/builtins.hpp
 * @brief Some built-in types.
**/

#pragma once
#ifndef DEMOLANG_BUILTINS
#define DEMOLANG_BUILTINS

#include <any>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <memory>


namespace DemoLang {

namespace ValueTypes {

/**
 * @brief Base class for all value types.
**/
class BaseType {
private:
    std::string name;
    std::any value;

public:
    BaseType();
    virtual bool operator==(const BaseType& other) const;
    virtual bool operator!=(const BaseType& other) const;
    virtual std::string getName() const = 0;
    virtual std::any getValue() const = 0;
    virtual std::string toString() const = 0;
    virtual std::shared_ptr<BaseType> clone() const = 0;
};


/**
 * @brief Integer value type.
**/
class Integer : public BaseType {
private:
    std::string name;
    long long value;

public:
    Integer();
    explicit Integer(long long val);
    std::string getName() const override;
    std::any getValue() const override;
    std::string toString() const override;
    std::shared_ptr<BaseType> clone() const override;
};


/**
 * @brief Float value type.
**/
class Float : public BaseType {
private:
    std::string name;
    long double value;

public:
    Float();
    explicit Float(long double val);
    std::string getName() const override;
    std::any getValue() const override;
    std::string toString() const override;
    std::shared_ptr<BaseType> clone() const override;
};


/**
 * @brief String value type.
**/
class String : public BaseType {
private:
    std::string name;
    std::string value;

public:
    String();
    explicit String(const std::string& val);
    std::string getName() const override;
    std::any getValue() const override;
    std::string toString() const override;
    std::shared_ptr<BaseType> clone() const override;
};

/**
 * @brief Exception value type.
**/
class Exception : public BaseType {
private:
    std::string name;
    std::string value;

public:
    Exception();
    explicit Exception(const std::string& val);
    std::string getName() const override;
    std::any getValue() const override;
    std::string toString() const override;
    std::shared_ptr<BaseType> clone() const override;
};


/**
 * @brief List value type.
**/
class List : public BaseType {
private:
    std::string name;
    std::vector<std::shared_ptr<BaseType>> value;

public:
    List();
    explicit List(const std::vector<std::shared_ptr<BaseType>>& val);
    std::string getName() const override;
    std::any getValue() const override;
    std::string toString() const override;
    std::shared_ptr<BaseType> clone() const override;
};


/**
 * @brief Get built-in functions map.
 * @return Map of built-in function names to their implementations.
**/
std::unordered_map<std::string, std::function<std::shared_ptr<BaseType>(const std::vector<std::shared_ptr<BaseType>>)>> getBuiltins();

} // namespace ValueTypes

} // namespace DemoLang

#endif // DEMOLANG_BUILTINS