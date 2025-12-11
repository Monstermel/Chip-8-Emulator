#ifndef CHIP_8_CHIP_8_STATE_HPP
#define CHIP_8_CHIP_8_STATE_HPP

#include <array>
#include <cstdint>
#include <stack>

namespace emu {

struct ChipState {
    // Memory
    std::array<std::uint8_t, 4096> memory{};
    // Registers
    std::array<std::uint8_t, 16> V{};
    // Program counter
    std::uint16_t program_counter{};
    // Index register
    std::uint16_t index_register{};
    // Delay Timer
    std::uint8_t delay_timer{};
    // Sound Timer
    std::uint8_t sound_timer{};

    // Stack
    std::stack<std::uint16_t> stack;
};

}  // namespace emu

#endif /* CHIP_8_CHIP_8_STATE_HPP */
