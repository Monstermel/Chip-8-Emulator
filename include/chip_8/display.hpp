#ifndef CHIP_8_DISPLAY_HPP
#define CHIP_8_DISPLAY_HPP

#include <array>
#include <cstddef>
#include <cstdint>

namespace emu::display {  // Registers metadata
// X axis
constexpr std::size_t kWidth = 64;
// Y axis
constexpr std::size_t kHeight = 32;

using Type = std::array<std::uint8_t, kWidth * kHeight>;

}  // namespace emu::display

#endif /* CHIP_8_DISPLAY_HPP */
