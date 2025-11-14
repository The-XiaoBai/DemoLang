/**
 * @file include/builtins.hpp
 * @brief Some built-in types.
 * @author The-XiaoBai
 * @date 2025/11/14
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


class AType : public BaseType {
private:
    std::string name = "A-Type";
    std::string value;

public:
    AType() = default;
    explicit AType(std::string val) : value(val) {}
    std::string getName() const override { return name; }
    std::any getValue() const override { return value; }
    std::shared_ptr<BaseType> clone() const override { return std::make_shared<AType>(value); }
};

} // namespace ValueTypes

} // namespace DemoLang

#endif // DEMOLANG_BUILTINS