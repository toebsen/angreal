//
// Created by toebsen on 26.11.2020.
//
#include "virtual_machine.h"

#include "chunk.h"
#include "debug.h"
#include "value.h"

#define READ_BYTE() (*ip_++)
#define READ_CONSTANT() (chunk_->Constants().Get(READ_BYTE()))
#define BINARY_OP(op)                    \
    do {                                 \
        auto b = value_stack_.pop_one(); \
        auto a = value_stack_.pop_one(); \
        value_stack_.push(a op b);       \
    } while (false)

namespace angreal {

VirtualMachine::VirtualMachine(DebugTracer& debug_tracer)
    : debug_tracer_(debug_tracer) {}

void VirtualMachine::Init() {
    chunk_ = nullptr;
    ip_ = nullptr;

    value_stack_.reset();
}

void VirtualMachine::DeInit() {}

InterpretResult VirtualMachine::Interpret(Chunk* chunk) {
    chunk_ = chunk;
    ip_ = chunk_->Begin();
    return Run();
}

InterpretResult VirtualMachine::Run() {
    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        debug_tracer_.TraceStack(value_stack_);
        debug_tracer_.DisassembleInstruction(
            chunk_, static_cast<size_t>(ip_ - chunk_->Begin()));
#endif
        switch (READ_BYTE()) {
            case AS_BYTE(OpCode::Constant): {
                value_t constant = READ_CONSTANT();
                value_stack_.push(constant);
                break;
            }
            case AS_BYTE(OpCode::Negate): {
                value_t constant = value_stack_.pop_one();
                value_stack_.push(-constant);
                break;
            }
            case AS_BYTE(OpCode::Add): {
                BINARY_OP(+);
                break;
            }
            case AS_BYTE(OpCode::Subtract): {
                BINARY_OP(-);
                break;
            }
            case AS_BYTE(OpCode::Multiply): {
                BINARY_OP(*);
                break;
            }
            case AS_BYTE(OpCode::Divide): {
                BINARY_OP(/);
                break;
            }
            case AS_BYTE(OpCode::Return): {
                if (!value_stack_.empty()) {
                    value_t constant = value_stack_.pop_one();
                    PrintValue(constant);
                    printf("\n");
                }
                return InterpretResult::Ok;
            }
        }
    }
}

}  // namespace angreal