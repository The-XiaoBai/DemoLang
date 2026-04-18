/**
 * @file src/interpreter/builtins.cpp
 * @brief Built=in types and functions implementation.
**/

#include "builtins.hpp"
#include <iostream>
#include <string>

namespace DemoLang {
namespace ValueTypes {

BaseType::BaseType() = default;
Integer::Integer() : name("Integer"), value(0) {}
Float::Float() : name("Float"), value(0.0) {}
String::String() : name("String"), value("") {}
Exception::Exception() : name("Exception"), value("") {}
List::List() : name("List"), value(std::vector<std::shared_ptr<BaseType>>()) {}

bool BaseType::operator==(const BaseType& other) const {
    return this->getName() == other.getName() &&
        std::any_cast<std::string>(this->getValue()) == std::any_cast<std::string>(other.getValue());
}

bool BaseType::operator!=(const BaseType& other) const {
    return this->getName() != other.getName() ||
        std::any_cast<std::string>(this->getValue()) != std::any_cast<std::string>(other.getValue());
}

Integer::Integer(long long val) : name("Integer"), value(val) {}

std::string Integer::getName() const {
    return name;
}

std::any Integer::getValue() const {
    return value;
}

std::shared_ptr<BaseType> Integer::clone() const {
    return std::make_shared<Integer>(value);
}

Float::Float(long double val) : name("Float"), value(val) {}

std::string Float::getName() const {
    return name;
}

std::any Float::getValue() const {
    return value;
}

std::shared_ptr<BaseType> Float::clone() const {
    return std::make_shared<Float>(value);
}

String::String(const std::string& val) : name("String"), value(val) {}

std::string String::getName() const {
    return name;
}

std::any String::getValue() const {
    return value;
}

std::shared_ptr<BaseType> String::clone() const {
    return std::make_shared<String>(value);
}

Exception::Exception(const std::string& val) : name("Exception"), value(val) {}

std::string Exception::getName() const {
    return name;
}

std::any Exception::getValue() const {
    return value;
}

std::shared_ptr<BaseType> Exception::clone() const {
    return std::make_shared<Exception>(value);
}

List::List(const std::vector<std::shared_ptr<BaseType>>& val) : name("List"), value(val) {}

std::string List::getName() const {
    return name;
}

std::any List::getValue() const {
    return value;
}

std::shared_ptr<BaseType> List::clone() const {
    return std::make_shared<List>(value);
}

// Helper to convert BaseType to string
std::string toString(const std::shared_ptr<BaseType>& val) {
    if (auto str = dynamic_cast<String*>(val.get())) {
        return std::any_cast<std::string>(str->getValue());
    } else if (auto integer = dynamic_cast<Integer*>(val.get())) {
        return std::to_string(std::any_cast<long long>(integer->getValue()));
    } else if (auto flo = dynamic_cast<Float*>(val.get())) {
        return std::to_string(std::any_cast<long double>(flo->getValue()));
    } else if (auto list = dynamic_cast<List*>(val.get())) {
        auto items = std::any_cast<std::vector<std::shared_ptr<BaseType>>>(list->getValue());
        std::string res = "[";
        for (size_t i = 0; i < items.size(); ++i) {
            res += toString(items[i]);
            if (i < items.size() - 1) res += ", ";
        }
        res += "]";
        return res;
    }
    return "?";
}

std::unordered_map<std::string, std::function<std::shared_ptr<BaseType>(const std::vector<std::shared_ptr<BaseType>>)>> getBuiltins() {
    std::unordered_map<std::string, std::function<std::shared_ptr<BaseType>(const std::vector<std::shared_ptr<BaseType>>)>> builtins;
    builtins["print"] = [](const std::vector<std::shared_ptr<BaseType>>& args) -> std::shared_ptr<BaseType> {
        for (size_t i = 0; i < args.size(); ++i) {
            std::cout << toString(args[i]);
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
