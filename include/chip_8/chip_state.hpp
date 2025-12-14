#ifndef CHIP_8_CHIP_8_STATE_HPP
#define CHIP_8_CHIP_8_STATE_HPP

#include <array>
#include <cstdint>
#include <random>
#include <stack>

namespace emu {

namespace display {                  // Registers metadata
constexpr std::size_t kWidth = 64;   // X
constexpr std::size_t kHeight = 32;  // Y
}  // namespace display

namespace font {  // Font metadata
constexpr std::uint16_t kSpriteSize = 5;
constexpr std::uint16_t kMemoryOffset = 0x000;
}  // namespace font

namespace memory {  // Memory layout
constexpr std::uint16_t kInterpreterSpaceOffset = 0x000;
constexpr std::uint16_t kProgramSpaceOffset = 0x200;
constexpr std::size_t kSize = 4096;
}  // namespace memory

namespace registers {  // Registers metadata
constexpr std::size_t kNum = 16;
}

struct ChipState {
    // Memory
    std::array<std::uint8_t, memory::kSize> memory{
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
    // Display buffer
    std::array<std::uint8_t, display::kWidth * display::kHeight> display{};
    // Registers
    std::array<std::uint8_t, registers::kNum> V{};
    // Random engine
    std::minstd_rand rnd;
    // Program counter
    std::uint16_t program_counter{memory::kProgramSpaceOffset};
    // Index register
    std::uint16_t index_register{};
    // Delay Timer
    std::uint8_t delay_timer{};
    // Sound Timer
    std::uint8_t sound_timer{};

    // Stack TODO: Implement a static stack
    std::stack<std::uint16_t> stack;
};

}  // namespace emu

#endif /* CHIP_8_CHIP_8_STATE_HPP */
