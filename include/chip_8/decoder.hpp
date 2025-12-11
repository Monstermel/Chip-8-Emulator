#ifndef CHIP_8_DECODER_HPP
#define CHIP_8_DECODER_HPP

#include <stdexcept>

#include "chip_8/instruction_set.hpp"

namespace emu {

class InvalidInstructionError : public std::runtime_error {
   public:
    explicit InvalidInstructionError()
        : std::runtime_error("Invalid instruction") {};
    explicit InvalidInstructionError(const std::string& message)
        : std::runtime_error(message) {}
};

inline instruction_set::Instruction handleGroup0(
    const std::uint16_t instruction) {
    switch (instruction & 0x0FFF) {
        case 0x00E0:
            return instruction_set::op00E0;
        case 0x00EE:
            return instruction_set::op00EE;
        default:
            return instruction_set::op0nnn;
    }
}

inline instruction_set::Instruction handleGroup1(
    const std::uint16_t /* not used */) {
    return instruction_set::op1nnn;
}

inline instruction_set::Instruction handleGroup2(
    const std::uint16_t /* not used */) {
    return instruction_set::op2nnn;
}

inline instruction_set::Instruction handleGroup3(
    const std::uint16_t /* not used */) {
    return instruction_set::op3xkk;
}

inline instruction_set::Instruction handleGroup4(
    const std::uint16_t /* not used */) {
    return instruction_set::op4xkk;
}

inline instruction_set::Instruction handleGroup5(
    const std::uint16_t /* not used */) {
    return instruction_set::op5xy0;
}

inline instruction_set::Instruction handleGroup6(
    const std::uint16_t /* not used */) {
    return instruction_set::op6xkk;
}

inline instruction_set::Instruction handleGroup7(
    const std::uint16_t /* not used */) {
    return instruction_set::op7xkk;
}

inline instruction_set::Instruction handleGroup8(
    const std::uint16_t instruction) {
    switch (instruction & 0x000F) {
        case 0x0000:
            return instruction_set::op8xy0;
        case 0x0001:
            return instruction_set::op8xy1;
        case 0x0002:
            return instruction_set::op8xy2;
        case 0x0003:
            return instruction_set::op8xy3;
        case 0x0004:
            return instruction_set::op8xy4;
        case 0x0005:
            return instruction_set::op8xy5;
        case 0x0006:
            return instruction_set::op8xy6;
        case 0x0007:
            return instruction_set::op8xy7;
        case 0x000E:
            return instruction_set::op8xyE;
        default:
            throw InvalidInstructionError();
    }
}

inline instruction_set::Instruction handleGroup9(
    const std::uint16_t /* not used */) {
    return instruction_set::op9xy0;
}

inline instruction_set::Instruction handleGroupA(
    const std::uint16_t /* not used */) {
    return instruction_set::opAnnn;
}

inline instruction_set::Instruction handleGroupB(
    const std::uint16_t /* not used */) {
    return instruction_set::opBnnn;
}

inline instruction_set::Instruction handleGroupC(
    const std::uint16_t /* not used */) {
    return instruction_set::opCxkk;
}

inline instruction_set::Instruction handleGroupD(
    const std::uint16_t /* not used */) {
    return instruction_set::opDxyn;
}

inline instruction_set::Instruction handleGroupE(
    const std::uint16_t instruction) {
    switch (instruction & 0x00FF) {
        case 0x009E:
            return instruction_set::opEx9E;
        case 0x00A1:
            return instruction_set::opExA1;
        default:
            throw InvalidInstructionError();
    }
}

inline instruction_set::Instruction handleGroupF(
    const std::uint16_t instruction) {
    switch (instruction & 0x00FF) {
        case 0x0007:
            return instruction_set::opFx07;
        case 0x000A:
            return instruction_set::opFx0A;
        case 0x0015:
            return instruction_set::opFx15;
        case 0x0018:
            return instruction_set::opFx18;
        case 0x001E:
            return instruction_set::opFx1E;
        case 0x0029:
            return instruction_set::opFx29;
        case 0x0033:
            return instruction_set::opFx33;
        case 0x0055:
            return instruction_set::opFx55;
        case 0x0065:
            return instruction_set::opFx65;
        default:
            throw InvalidInstructionError();
    }
}

inline instruction_set::Instruction decode(const std::uint16_t instruction) {
    switch (instruction & 0xF000U) {
        case 0x0000:
            return handleGroup0(instruction);
        case 0x1000:
            return handleGroup1(instruction);
        case 0x2000:
            return handleGroup2(instruction);
        case 0x3000:
            return handleGroup3(instruction);
        case 0x4000:
            return handleGroup4(instruction);
        case 0x5000:
            return handleGroup5(instruction);
        case 0x6000:
            return handleGroup6(instruction);
        case 0x7000:
            return handleGroup7(instruction);
        case 0x8000:
            return handleGroup8(instruction);
        case 0x9000:
            return handleGroup9(instruction);
        case 0xA000:
            return handleGroupA(instruction);
        case 0xB000:
            return handleGroupB(instruction);
        case 0xC000:
            return handleGroupC(instruction);
        case 0xD000:
            return handleGroupD(instruction);
        case 0xE000:
            return handleGroupE(instruction);
        case 0xF000:
            return handleGroupF(instruction);
        default:
            throw InvalidInstructionError();
    }
}

}  // namespace emu

#endif /* CHIP_8_DECODER_HPP */
