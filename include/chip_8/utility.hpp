#ifndef CHIP_8_UTILITY_HPP
#define CHIP_8_UTILITY_HPP

#include <cstdint>

namespace emu {

inline std::uint8_t getNibbleX(const std::uint16_t instruction) {
    return (instruction & 0x0F00) >> 8;
}

inline std::uint8_t getNibbleY(const std::uint16_t instruction) {
    return (instruction & 0x00F0) >> 4;
}

inline std::uint8_t getLowByte(const std::uint16_t instruction) {
    return instruction & 0x00FF;
}

inline std::uint16_t getAddress(const std::uint16_t instruction) {
    return instruction & 0x0FFF;
}

};  // namespace emu

#endif /* CHIP_8_UTILITY_HPP */
