/**
 * @file include/builtins.hpp
 * @brief Some built-in types.
**/

#pragma once
#ifndef DEMOLANG_BUILTINS
#define DEMOLANG_BUILTINS

#include <any>
#include <iostream>
#include <unordered_map>
#include <functional>


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

inline std::unordered_map<std::string, std::function<std::shared_ptr<BaseType>(const std::vector<std::shared_ptr<BaseType>>)>> getBuiltins() {
    std::unordered_map<std::string, std::function<std::shared_ptr<BaseType>(const std::vector<std::shared_ptr<BaseType>>)>> builtins;
    builtins["print"] = [](const std::vector<std::shared_ptr<BaseType>>& args) -> std::shared_ptr<BaseType> {
        for (size_t i = 0; i < args.size(); ++i) {
            if (auto str = dynamic_cast<String*>(args[i].get())) {
                std::cout << std::any_cast<std::string>(str->getValue());
            } else if (auto integer = dynamic_cast<Integer*>(args[i].get())) {
                std::cout << std::any_cast<long long>(integer->getValue());
            } else if (auto flo = dynamic_cast<Float*>(args[i].get())) {
                std::cout << std::any_cast<long double>(flo->getValue());
            }
            if (i < args.size() - 1) std::cout << " ";
        }
        std::cout << std::endl;
        return std::make_shared<String>("");
    };
    builtins["exit"] = [](const std::vector<std::shared_ptr<BaseType>>& args) -> std::shared_ptr<BaseType> {
        int code = 0;
        if (!args.empty()) {
            if (auto integer = dynamic_cast<Integer*>(args[0].get())) {
                code = static_cast<int>(std::any_cast<long long>(integer->getValue()));
            }
        }
        exit(code);
        return std::make_shared<String>("");
    };
    builtins["query"] = [](const std::vector<std::shared_ptr<BaseType>>& args) -> std::shared_ptr<BaseType> {
        std::string input;
        std::getline(std::cin, input);
        return std::make_shared<String>(input);
    };
    return builtins;
}

} // namespace ValueTypes

} // namespace DemoLang

#endif // DEMOLANG_BUILTINS