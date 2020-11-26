//
// Created by toebsen on 26.11.2020.
//
#ifndef ANGREAL_SRC_LIBANGREAL_VIRTUAL_MACHINE_H_
#define ANGREAL_SRC_LIBANGREAL_VIRTUAL_MACHINE_H_

#include "common.h"

namespace angreal {

class Chunk;

enum class InterpretResult : uint8_t {
    Ok,
    CompileError,
    RuntimeError,
};

class VirtualMachine final {
   public:
    VirtualMachine() = default;

    VirtualMachine(const VirtualMachine& other) = delete;
    VirtualMachine(VirtualMachine&& other) = delete;
    VirtualMachine& operator=(const VirtualMachine& other) = delete;
    VirtualMachine& operator=(VirtualMachine&& other) = delete;

    void Init();

    void DeInit();

    InterpretResult Interpret(Chunk* chunk);

   protected:
    InterpretResult Run();

    PRIVATE : Chunk* chunk_;
    uint8_t* ip_;
};
}  // namespace angreal

#endif  // ANGREAL_SRC_LIBANGREAL_VIRTUAL_MACHINE_H_