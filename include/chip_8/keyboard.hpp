#ifndef CHIP_8_KEY_MAP_HPP
#define CHIP_8_KEY_MAP_HPP

#include <cstddef>
#include <cstdint>

#include "SDL3/SDL_scancode.h"

namespace emu::keyboard {

constexpr unsigned int kNumKeys = 16;

inline std::size_t mapping(const std::uint8_t key) {
    switch (key) {
        // First row
        case 0x1U:
            return SDL_SCANCODE_1;
        case 0x2U:
            return SDL_SCANCODE_2;
        case 0x3U:
            return SDL_SCANCODE_3;
        case 0xCU:
            return SDL_SCANCODE_4;
        // Second row
        case 0x4U:
            return SDL_SCANCODE_Q;
        case 0x5U:
            return SDL_SCANCODE_W;
        case 0x6U:
            return SDL_SCANCODE_E;
        case 0xDU:
            return SDL_SCANCODE_R;
        // Third row
        case 0x7U:
            return SDL_SCANCODE_A;
        case 0x8U:
            return SDL_SCANCODE_S;
        case 0x9U:
            return SDL_SCANCODE_D;
        case 0xEU:
            return SDL_SCANCODE_F;
        // Forth row
        case 0xAU:
            return SDL_SCANCODE_Z;
        case 0x0U:
            return SDL_SCANCODE_X;
        case 0xBU:
            return SDL_SCANCODE_C;
        case 0xFU:
            return SDL_SCANCODE_V;
        // Any other key
        default:
            return SDL_SCANCODE_UNKNOWN;
    }
}

using Type = const bool*;

};  // namespace emu::keyboard

#endif /* CHIP_8_KEY_MAP_HPP */
