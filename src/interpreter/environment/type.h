//
// Created by toebs on 04.04.2020.
//

#ifndef ANGREAL_SRC_INTERPRETER_VIRTUAL_MACHINE_TYPE_H_
#define ANGREAL_SRC_INTERPRETER_VIRTUAL_MACHINE_TYPE_H_

#include <memory>
#include <variant>

#include "callable.h"
#include "common.h"
#include "object.h"

namespace angreal::interpreter::environment {

class Type {
    using value_t =
        std::variant<std::nullptr_t, bool, int, float, string_t, callable_t>;

   public:
    explicit Type() : value_(nullptr) {}
    explicit Type(value_t value) : value_(std::move(value)) {}
    //    explicit Type(CallablePtr value) : value_(std::move(value)) {}

    virtual bool IsNull() {
        return std::holds_alternative<std::nullptr_t>(value_);
    }
    virtual bool IsBoolean() { return std::holds_alternative<bool>(value_); }
    virtual bool IsFloat() { return std::holds_alternative<float>(value_); };
    virtual bool IsInteger() { return std::holds_alternative<int>(value_); };
    virtual bool IsString() {
        return std::holds_alternative<string_t>(value_);
    };
    virtual bool IsCallable() {
        return std::holds_alternative<callable_t>(value_);
    };

    virtual bool AsBoolean() { return std::get<bool>(value_); };
    virtual float AsFloat() { return std::get<float>(value_); };
    virtual int AsInteger() { return std::get<int>(value_); };

    virtual bool IsTruthy() {
        // TODO(toebs): add missing
        if (IsBoolean()) {
            return AsBoolean();
        }

        return false;
    };

    virtual string_t AsString() { return std::get<string_t>(value_); };
    virtual callable_t AsCallable() { return std::get<callable_t>(value_); };

    string_t Stringify() {
        std::stringstream ss;
        if (IsBoolean()) {
            ss << std::boolalpha << AsBoolean();
        }
        if (IsFloat()) {
            ss << AsFloat();
        }
        if (IsInteger()) {
            ss << AsInteger();
        }
        if (IsNull()) {
            ss << "None";
        }
        if (IsString()) {
            ss << "\"" << AsString() << "\"";
        }
        return ss.str();
    }

    [[nodiscard]] inline bool HasSameType(const Type& rhs) const {
        return value_.index() == rhs.value_.index();
    };

   private:
    value_t value_;
};  // namespace angreal::interpreter::environment

}  // namespace angreal::interpreter::environment

#endif  // ANGREAL_SRC_INTERPRETER_VIRTUAL_MACHINE_TYPE_H_
