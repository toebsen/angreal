//
// Created by toebsen on 26.11.2020.
//
#ifndef ANGREAL_SRC_LIBANGREAL_VIRTUAL_MACHINE_H_
#define ANGREAL_SRC_LIBANGREAL_VIRTUAL_MACHINE_H_

#include "common.h"
#include "stack.h"
#include "value.h"

namespace angreal {

class Chunk;

enum class InterpretResult : uint8_t {
    Ok,
    CompileError,
    RuntimeError,
};

class StackTracer;

class VirtualMachine final {
   public:
    using ValueStack = Stack<value_t>;

    VirtualMachine(StackTracer& stack_tracer);

    VirtualMachine(const VirtualMachine& other) = delete;
    VirtualMachine(VirtualMachine&& other) = delete;
    VirtualMachine& operator=(const VirtualMachine& other) = delete;
    VirtualMachine& operator=(VirtualMachine&& other) = delete;

    void Init();

    void DeInit();

    InterpretResult Interpret(Chunk* chunk);

    PRIVATE : Chunk* chunk_;
    uint8_t* ip_;
    ValueStack value_stack_;
    StackTracer& stack_tracer_;

   private:
    InterpretResult Run();
};

class StackTracer {
   public:
    virtual ~StackTracer() = default;

    virtual void TraceStack(const Stack<value_t>& stack);
};

}  // namespace angreal

#endif  // ANGREAL_SRC_LIBANGREAL_VIRTUAL_MACHINE_H_