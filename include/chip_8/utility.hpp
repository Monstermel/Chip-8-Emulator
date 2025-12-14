#ifndef CHIP_8_UTILITY_HPP
#define CHIP_8_UTILITY_HPP

#include <cstdint>

namespace emu {

constexpr unsigned int kByteWidth = 8U;

constexpr unsigned int kNibbleXMask = 0x0F00U;

constexpr unsigned int kNibbleYMask = 0x00F0U;

constexpr unsigned int kNibbleNMask = 0x000FU;

constexpr unsigned int kLowByteMask = 0x00FFU;

constexpr unsigned int kAddressMask = 0x0FFFU;

/**
 * @brief Get the X nibble from bytecode
 *
 * @param bytecode
 * @return std::uint8_t
 */
inline std::uint8_t getNibbleX(const std::uint16_t bytecode) {
    return (bytecode & kNibbleXMask) >> kByteWidth;
}

/**
 * @brief Get the Y nibble from bytecode
 *
 * @param bytecode
 * @return std::uint8_t
 */
inline std::uint8_t getNibbleY(const std::uint16_t bytecode) {
    return (bytecode & kNibbleYMask) >> (kByteWidth >> 1U);
}

/**
 * @brief Get the N nibble from bytecode
 *
 * @param bytecode
 * @return std::uint8_t
 */
inline std::uint8_t getNibbleN(const std::uint16_t bytecode) {
    return (bytecode & kNibbleNMask);
}

/**
 * @brief Get the low byte from bytecode
 *
 * @param bytecode
 * @return std::uint8_t
 */
inline std::uint8_t getLowByte(const std::uint16_t bytecode) {
    return bytecode & kLowByteMask;
}

/**
 * @brief Get the address from bytecode (three low nibbles)
 *
 * @param bytecode
 * @return std::uint8_t
 */
inline std::uint16_t getAddress(const std::uint16_t bytecode) {
    return bytecode & kAddressMask;
}

};  // namespace emu

#endif /* CHIP_8_UTILITY_HPP */
