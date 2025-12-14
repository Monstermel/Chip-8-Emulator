#ifndef CHIP_8_REGISTERS_HPP
#define CHIP_8_REGISTERS_HPP

#include <array>
#include <cstddef>

namespace emu::registers {  // Registers metadata

constexpr std::size_t kNum = 16;

using Type = std::array<std::uint8_t, registers::kNum>;

}  // namespace emu::registers

#endif /* CHIP_8_REGISTERS_HPP */
