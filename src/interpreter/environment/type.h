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
    using value_t = std::variant<std::nullptr_t, bool, int, float, string_t,
                                 callable_t, instance_t>;

   public:
    explicit Type() : value_(nullptr) {}
    explicit Type(value_t value) : value_(std::move(value)) {}
    //    explicit Type(CallablePtr value) : value_(std::move(value)) {}

    inline bool IsNull() {
        return std::holds_alternative<std::nullptr_t>(value_);
    }
    inline bool IsBoolean() { return std::holds_alternative<bool>(value_); }
    inline bool IsFloat() { return std::holds_alternative<float>(value_); };
    inline bool IsInteger() { return std::holds_alternative<int>(value_); };
    inline bool IsString() { return std::holds_alternative<string_t>(value_); };
    inline bool IsCallable() {
        return std::holds_alternative<callable_t>(value_);
    };
    inline bool IsInstance() {
        return std::holds_alternative<instance_t>(value_);
    };

    inline bool AsBoolean() { return std::get<bool>(value_); };
    inline float AsFloat() { return std::get<float>(value_); };
    inline int AsInteger() { return std::get<int>(value_); };

    inline string_t AsString() { return std::get<string_t>(value_); };
    inline callable_t AsCallable() { return std::get<callable_t>(value_); };
    inline instance_t AsInstance() { return std::get<instance_t>(value_); };

    inline value_t value() { return value_; }

    bool IsTruthy() {
        return std::visit(
            Overloaded {[](std::nullptr_t val) { return false; },
                        [](bool val) { return val; },
                        [](int i) { return static_cast<bool>(i); },
                        [](float f) { return static_cast<bool>(f); },
                        [](string_t s) { return !s.empty(); },
                        [](callable_t c) { return true; },
                        [](instance_t i) { return true; }},
            value_);
    };

    string_t Stringify() {
        return std::visit(
            Overloaded {[](std::nullptr_t val) { return string_t("None"); },
                        [](bool val) {
                            return val ? string_t("true") : string_t("false");
                        },
                        [](int i) { return std::to_string(i); },
                        [](float f) { return std::to_string(f); },
                        [](string_t s) { return string_t("\"" + s + "\""); },
                        [](callable_t c) { return c->Stringify(); },
                        [](instance_t i) { return i->Stringify(); }},
            value_);
    }

    [[nodiscard]] inline bool HasSameType(const Type& rhs) const {
        return value_.index() == rhs.value_.index();
    };

   private:
    value_t value_;
};  // namespace angreal::interpreter::environment

}  // namespace angreal::interpreter::environment

#endif  // ANGREAL_SRC_INTERPRETER_VIRTUAL_MACHINE_TYPE_H_
