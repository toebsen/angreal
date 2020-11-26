//
// Created by toebsen on 26.11.2020.
//
#include "virtual_machine.h"

#include "chunk.h"
#include "value.h"

namespace angreal {

void VirtualMachine::Init() {
    chunk_ = nullptr;
    ip_ = nullptr;
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
        switch (READ_BYTE()) {
            case AS_BYTE(OpCode::Constant): {
                value_t constant = READ_CONSTANT();
                PrintValue(constant);
                printf("\n");
                break;
            }
            case AS_BYTE(OpCode::Return): {
                return InterpretResult::Ok;
            }
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
}

}  // namespace angreal