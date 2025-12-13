#ifndef CHIP_8_CHIP_8_STATE_HPP
#define CHIP_8_CHIP_8_STATE_HPP

#include <array>
#include <cstdint>
#include <random>
#include <stack>

namespace emu {

// Font metadata
constexpr std::uint16_t kFontSpriteSize = 5;
constexpr std::uint16_t kFontMemoryOffset = 0x000;

// Memory layout
constexpr std::uint16_t kInterpreterSpaceOffset = 0x000;
constexpr std::uint16_t kProgramSpaceOffset = 0x200;
constexpr std::size_t kMemorySize = 4096;

// Registers metadata
constexpr std::size_t kNumberRegisters = 16;

struct ChipState {
    // Memory
    std::array<std::uint8_t, kMemorySize> memory{
        0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
        0x20, 0x60, 0x20, 0x20, 0x70,  // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
        0xF0, 0x80, 0xF0, 0x80, 0x80   // F
    };
    // Registers
    std::array<std::uint8_t, kNumberRegisters> V{};
    // Random engine
    std::minstd_rand rnd;
    // Program counter
    std::uint16_t program_counter{kProgramSpaceOffset};
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
