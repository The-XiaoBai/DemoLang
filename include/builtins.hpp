/**
 * @file include/builtins.hpp
 * @brief Some built-in types.
 * @author The-XiaoBai
 * @date 2025/12/12
**/

#pragma once
#ifndef DEMOLANG_BUILTINS
#define DEMOLANG_BUILTINS

#include <any>
#include <iostream>


namespace DemoLang {

namespace ValueTypes {

class BaseType {
private:
    std::string name;
    std::any value;

public:
    BaseType() = default;
    virtual bool operator==(const BaseType& other) const {
        return this->getName() == other.getName() &&
            std::any_cast<std::string>(this->getValue()) == std::any_cast<std::string>(other.getValue());
    }
    virtual bool operator!=(const BaseType& other) const {
        return this->getName() != other.getName() ||
            std::any_cast<std::string>(this->getValue()) != std::any_cast<std::string>(other.getValue());
    }
    virtual std::string getName() const = 0;
    virtual std::any getValue() const = 0;
    virtual std::shared_ptr<BaseType> clone() const = 0;
};


class Integer : public BaseType {
private:
    std::string name = "Integer";
    long long value;

public:
    Integer() = default;
    explicit Integer(long long val) : value(val) {}
    std::string getName() const override { return name; }
    std::any getValue() const override { return value; }
    std::shared_ptr<BaseType> clone() const override {
        return std::make_shared<Integer>(value);
    }
};


class Float : public BaseType {
private:
    std::string name = "Float";
    long double value;

public:
    Float() = default;
    explicit Float(long double val) : value(val) {}
    std::string getName() const override { return name; }
    std::any getValue() const override { return value; }
    std::shared_ptr<BaseType> clone() const override {
        return std::make_shared<Float>(value);
    }
};


class String : public BaseType {
private:
    std::string name = "String";
    std::string value;

public:
    String() = default;
    explicit String(const std::string& val) : value(val) {}
    std::string getName() const override { return name; }
    std::any getValue() const override { return value; }
    std::shared_ptr<BaseType> clone() const override {
        return std::make_shared<String>(value);
    }
};

class Exception : public BaseType {
private:
    std::string name = "Exception";
    std::string value;

public:
    Exception() = default;
    explicit Exception(const std::string& val) : value(val) {}
    std::string getName() const override { return name; }
    std::any getValue() const override { return value; }
    std::shared_ptr<BaseType> clone() const override {
        return std::make_shared<Exception>(value);
    }
};

} // namespace ValueTypes

} // namespace DemoLang

#endif // DEMOLANG_BUILTINS