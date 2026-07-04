/**
 * @file src/interpreter/builtins.cpp
 * @brief Built-in types and functions implementation.
**/

#include <iostream>
#include <string>
#include "builtins.hpp"
#include "utils.hpp"

namespace DemoLang {
namespace ValueTypes {

// ── Constructors ────────────────────────────────────────────────────────────

Integer::Integer() : BaseType("Integer"), value(0) {}
Float::Float()   : BaseType("Float"),   value(0.0) {}
String::String() : BaseType("String"),  value("") {}
Exception::Exception() : BaseType("Exception"), value("") {}
List::List()     : BaseType("List"),    value(std::vector<std::shared_ptr<BaseType>>()) {}

Integer::Integer(long long val) : BaseType("Integer"), value(val) {}
Float::Float(long double val)   : BaseType("Float"),   value(val) {}
String::String(const std::string& val)  : BaseType("String"),  value(val) {}
Exception::Exception(const std::string& val) : BaseType("Exception"), value(val) {}
List::List(const std::vector<std::shared_ptr<BaseType>>& val) : BaseType("List"), value(val) {}

// ── Equality ────────────────────────────────────────────────────────────────

bool BaseType::operator==(const BaseType& other) const {
    return getName() == other.getName();
}

bool BaseType::operator!=(const BaseType& other) const {
    return !(*this == other);
}

bool Integer::operator==(const BaseType& other) const {
    auto o = dynamic_cast<const Integer*>(&other);
    return o && value == o->value;
}

bool Float::operator==(const BaseType& other) const {
    auto o = dynamic_cast<const Float*>(&other);
    return o && value == o->value;
}

bool String::operator==(const BaseType& other) const {
    auto o = dynamic_cast<const String*>(&other);
    return o && value == o->value;
}

bool Exception::operator==(const BaseType& other) const {
    auto o = dynamic_cast<const Exception*>(&other);
    return o && value == o->value;
}

bool List::operator==(const BaseType& other) const {
    auto o = dynamic_cast<const List*>(&other);
    if (!o || value.size() != o->value.size()) return false;
    for (size_t i = 0; i < value.size(); ++i) {
        if (!(*value[i] == *o->value[i])) return false;
    }
    return true;
}

// ── Value accessors ─────────────────────────────────────────────────────────

std::any Integer::getValue() const { return value; }
std::any Float::getValue()   const { return value; }
std::any String::getValue()  const { return value; }
std::any Exception::getValue() const { return value; }
std::any List::getValue()    const { return value; }

// ── Clone ───────────────────────────────────────────────────────────────────

std::shared_ptr<BaseType> Integer::clone() const   { return std::make_shared<Integer>(value); }
std::shared_ptr<BaseType> Float::clone() const     { return std::make_shared<Float>(value); }
std::shared_ptr<BaseType> String::clone() const    { return std::make_shared<String>(value); }
std::shared_ptr<BaseType> Exception::clone() const { return std::make_shared<Exception>(value); }
std::shared_ptr<BaseType> List::clone() const      { return std::make_shared<List>(value); }

// ── toString ────────────────────────────────────────────────────────────────

std::string Integer::toString() const   { return std::to_string(value); }
std::string Float::toString() const     { return std::to_string(value); }
std::string String::toString() const    { return value; }
std::string Exception::toString() const { return value; }
std::string List::toString() const {
    std::string res = "[";
    for (size_t i = 0; i < value.size(); ++i) {
        res += value[i]->toString();
        if (i < value.size() - 1) res += ", ";
    }
    res += "]";
    return res;
}

// ── Builtin registry ────────────────────────────────────────────────────────

// Builtin registry type alias using Registry template
using BuiltinRegistry = Utils::Registry<
    std::string,
    std::shared_ptr<BaseType>,
    const std::vector<std::shared_ptr<BaseType>>&
>;

static bool builtinsInitialized = false;

static void initBuiltins() {
    if (builtinsInitialized) return;
    builtinsInitialized = true;
    auto& reg = BuiltinRegistry::instance();
    reg.registerFunc("query", [](const std::vector<std::shared_ptr<BaseType>>& args) -> std::shared_ptr<BaseType> {
        (void)args;
        std::string input;
        std::getline(std::cin, input);
        return std::make_shared<String>(input);
    });
    reg.registerFunc("print", [](const std::vector<std::shared_ptr<BaseType>>& args) -> std::shared_ptr<BaseType> {
        for (size_t i = 0; i < args.size(); ++i) {
            std::cout << args[i]->toString();
            if (i < args.size() - 1) std::cout << " ";
        }
        std::cout << std::endl;
        return std::make_shared<String>("");
    });
    reg.registerFunc("exit", [](const std::vector<std::shared_ptr<BaseType>>& args) -> std::shared_ptr<BaseType> {
        int code = 0;
        if (!args.empty()) {
            if (auto integer = dynamic_cast<Integer*>(args[0].get())) {
                code = static_cast<int>(std::any_cast<long long>(integer->getValue()));
            }
        }
        exit(code);
        return std::make_shared<String>("");
    });
}

std::shared_ptr<BaseType> getBuiltin(const std::string& name, const std::vector<std::shared_ptr<BaseType>>& args) {
    initBuiltins();
    auto& reg = BuiltinRegistry::instance();
    if (reg.isRegistered(name)) {
        return reg.execute(name, args);
    }
    return nullptr;  // Not a builtin — caller should check user-defined functions
}

} // namespace ValueTypes
} // namespace DemoLang
