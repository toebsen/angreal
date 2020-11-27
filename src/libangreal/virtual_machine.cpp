//
// Created by toebsen on 26.11.2020.
//
#include "virtual_machine.h"

#include "chunk.h"
#include "debug.h"
#include "value.h"

namespace angreal {

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
#define READ_BYTE() (*ip_++)
#define READ_CONSTANT() (chunk_->Constants().Get(READ_BYTE()))

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (auto it = value_stack_.cbegin(); it != value_stack_.cend(); it++) {
            printf("[ ");
            PrintValue((*it));
            printf(" ]");
        }
        printf("\n");
        debug::disassembleInstruction(
            chunk_, static_cast<size_t>(ip_ - chunk_->Begin()));
#endif
        switch (READ_BYTE()) {
            case AS_BYTE(OpCode::Constant): {
                value_t constant = READ_CONSTANT();
                value_stack_.push(constant);

                break;
            }
            case AS_BYTE(OpCode::Return): {
                value_t constant = value_stack_.top();
                value_stack_.pop();
                PrintValue(constant);
                printf("\n");
                return InterpretResult::Ok;
            }
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
}

}  // namespace angreal