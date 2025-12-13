#ifndef CHIP_8_UTILITY_HPP
#define CHIP_8_UTILITY_HPP

#include <cstdint>

namespace emu {

constexpr unsigned int kByteWidth = 8U;

constexpr unsigned int kNibbleXMask = 0x0F00U;

constexpr unsigned int kNibbleYMask = 0x00F0U;

constexpr unsigned int kLowByteMask = 0x00FFU;

constexpr unsigned int kAddressMask = 0x0FFFU;

inline std::uint8_t getNibbleX(const std::uint16_t bytecode) {
    return (bytecode & kNibbleXMask) >> kByteWidth;
}

inline std::uint8_t getNibbleY(const std::uint16_t bytecode) {
    return (bytecode & kNibbleYMask) >> (kByteWidth >> 1U);
}

inline std::uint8_t getLowByte(const std::uint16_t bytecode) {
    return bytecode & kLowByteMask;
}

inline std::uint16_t getAddress(const std::uint16_t bytecode) {
    return bytecode & kAddressMask;
}

};  // namespace emu

#endif /* CHIP_8_UTILITY_HPP */
