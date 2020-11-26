//
// Created by toebsen on 26.11.2020.
//

#include <iostream>

#include "chunk.h"
#include "debug.h"
#include "virtual_machine.h"

using namespace angreal;

int main(int argc, const char* argv[]) {
    VirtualMachine vm;

    vm.Init();
    Chunk chunk;

    auto constant = chunk.Constants().Write(1.2f);
    chunk.WriteChunk(OpCode::Constant, 123);
    chunk.WriteChunk(constant, 123);

    chunk.WriteChunk(OpCode::Return, 124);
    auto result = vm.Interpret(&chunk);

    std::cout << "Result=" << static_cast<int>(result) << std::endl;

    vm.DeInit();
    return 0;
}