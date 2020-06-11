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

    inline value_t value() { return value_; }
    inline bool IsTruthy() {
        // TODO(toebs): add missing
        if (IsBoolean()) {
            return AsBoolean();
        }

        return false;
    };

    inline string_t AsString() { return std::get<string_t>(value_); };
    inline callable_t AsCallable() { return std::get<callable_t>(value_); };
    inline instance_t AsInstance() { return std::get<instance_t>(value_); };

    string_t Stringify() {
        std::stringstream ss;

        std::visit(
            Overloaded {[&ss](std::nullptr_t val) { ss << "None"; },
                        [&ss](bool val) { ss << std::boolalpha << val; },
                        [&ss](int i) { ss << i; }, [&ss](float f) { ss << f; },
                        [&ss](string_t s) { ss << "\"" << s << "\""; },
                        [&ss](callable_t c) { ss << c->Stringify(); },
                        [&ss](instance_t i) { ss << i->Stringify(); }},
            value_);
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
