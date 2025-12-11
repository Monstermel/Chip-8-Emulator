#ifndef CHIP_8_CHIP_8_HPP
#define CHIP_8_CHIP_8_HPP

#include <cstdint>

#include "chip_8/chip_state.hpp"
#include "chip_8/decoder.hpp"

namespace emu {

class Chip8 {
    ChipState state_;

    std::uint16_t fetch() {
        const auto kInstruction = static_cast<std::uint16_t>(
            state_.memory[state_.program_counter] << 8U |
            state_.memory[state_.program_counter + 1]);

        state_.program_counter += 2;

        return kInstruction;
    }

   public:
    void cycle() {
        auto bytecode = fetch();

        auto instruction = decode(bytecode);

        instruction(state_, bytecode);
    }
};

}  // namespace emu

#endif /* CHIP_8_CHIP_8_HPP */
