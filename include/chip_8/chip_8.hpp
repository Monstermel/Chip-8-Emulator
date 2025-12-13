#ifndef CHIP_8_CHIP_8_HPP
#define CHIP_8_CHIP_8_HPP

#include <cstdint>

#include "chip_8/chip_state.hpp"
#include "chip_8/error.hpp"
#include "chip_8/instruction_set.hpp"
#include "chip_8/utility.hpp"

namespace emu {

class Chip8 {
    ChipState state_;

    std::uint16_t fetch() noexcept {
        const auto kInstruction =
            (static_cast<unsigned int>(state_.memory[state_.program_counter])
             << kByteWidth) |
            static_cast<unsigned int>(
                state_.memory[state_.program_counter + 1]);

        state_.program_counter += 2;

        return kInstruction;
    }

    static instruction_set::Instruction handleGroup0(
        const std::uint16_t instruction) {
        switch (instruction & 0x0FFFU) {
            case 0x00E0:
                return instruction_set::op00E0;
            case 0x00EE:
                return instruction_set::op00EE;
            default:
                return instruction_set::op0nnn;
        }
    }

    static instruction_set::Instruction handleGroup8(
        const std::uint16_t instruction) {
        switch (instruction & 0x000FU) {
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

    static instruction_set::Instruction handleGroupE(
        const std::uint16_t instruction) {
        switch (instruction & 0x00FFU) {
            case 0x009E:
                return instruction_set::opEx9E;
            case 0x00A1:
                return instruction_set::opExA1;
            default:
                throw InvalidInstructionError();
        }
    }

    static instruction_set::Instruction handleGroupF(
        const std::uint16_t instruction) {
        switch (instruction & 0x00FFU) {
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

    static instruction_set::Instruction decode(
        const std::uint16_t instruction) {
        switch (instruction & 0xF000U) {
            case 0x0000:
                return handleGroup0(instruction);
            case 0x1000:
                return instruction_set::op1nnn;
            case 0x2000:
                return instruction_set::op2nnn;
            case 0x3000:
                return instruction_set::op3xkk;
            case 0x4000:
                return instruction_set::op4xkk;
            case 0x5000:
                return instruction_set::op5xy0;
            case 0x6000:
                return instruction_set::op6xkk;
            case 0x7000:
                return instruction_set::op7xkk;
            case 0x8000:
                return handleGroup8(instruction);
            case 0x9000:
                return instruction_set::op9xy0;
            case 0xA000:
                return instruction_set::opAnnn;
            case 0xB000:
                return instruction_set::opBnnn;
            case 0xC000:
                return instruction_set::opCxkk;
            case 0xD000:
                return instruction_set::opDxyn;
            case 0xE000:
                return handleGroupE(instruction);
            case 0xF000:
                return handleGroupF(instruction);
            default:
                throw InvalidInstructionError();
        }
    }

   public:
    int load();

    void cycle() {
        const auto kBytecode = fetch();

        const auto kInstruction = decode(kBytecode);

        kInstruction(state_, kBytecode);

        if (state_.delay_timer > 0) {
            state_.delay_timer -= 1;
        }

        if (state_.sound_timer > 0) {
            state_.sound_timer -= 1;
        }
    }
};

}  // namespace emu

#endif /* CHIP_8_CHIP_8_HPP */
