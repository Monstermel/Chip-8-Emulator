#ifndef CHIP_8_MEMORY_HPP
#define CHIP_8_MEMORY_HPP

#include <array>
#include <cstdint>

namespace emu::memory {  // Memory layout

constexpr std::uint16_t kInterpreterSpaceOffset = 0x000;
constexpr std::uint16_t kProgramSpaceOffset = 0x200;
constexpr std::size_t kSize = 4096;

using Type = std::array<std::uint8_t, kSize>;

}  // namespace emu::memory

#endif /* CHIP_8_MEMORY_HPP */
