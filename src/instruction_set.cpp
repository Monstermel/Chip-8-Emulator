#include "chip_8/instruction_set.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "chip_8/display.hpp"
#include "chip_8/error.hpp"
#include "chip_8/keyboard.hpp"
#include "chip_8/utility.hpp"

#include "SDL3/SDL_log.h"

namespace emu::instruction_set {

void op0nnn(ChipState& /* not used */, const std::uint16_t /* not used */) {}

void op00E0(ChipState& state, const std::uint16_t /* not used */) {
    std::memset(state.display.data(), 0x00, state.display.size());
}

void op00EE(ChipState& state, const std::uint16_t /* not used */) {
    if (state.stack.empty()) {
        throw StackUnderflowError("Stack underflow on RET");
    }

    state.program_counter = state.stack.top();
    state.stack.pop();
}

void op1nnn(ChipState& state, const std::uint16_t bytecode) {
    state.program_counter = getAddress(bytecode);
}

void op2nnn(ChipState& state, const std::uint16_t bytecode) {
    state.stack.push(state.program_counter);
    state.program_counter = getAddress(bytecode);
}

void op3xkk(ChipState& state, const std::uint16_t bytecode) {
    if (state.V[getNibbleX(bytecode)] == getLowByte(bytecode)) {
        state.program_counter += 2;
    }
}

void op4xkk(ChipState& state, const std::uint16_t bytecode) {
    if (state.V[getNibbleX(bytecode)] != getLowByte(bytecode)) {
        state.program_counter += 2;
    }
}

void op5xy0(ChipState& state, const std::uint16_t bytecode) {
    if (state.V[getNibbleX(bytecode)] == state.V[getNibbleY(bytecode)]) {
        state.program_counter += 2;
    }
}

void op6xkk(ChipState& state, const std::uint16_t bytecode) {
    state.V[getNibbleX(bytecode)] = getLowByte(bytecode);
}

void op7xkk(ChipState& state, const std::uint16_t bytecode) {
    state.V[getNibbleX(bytecode)] += getLowByte(bytecode);
}

void op8xy0(ChipState& state, const std::uint16_t bytecode) {
    state.V[getNibbleX(bytecode)] = state.V[getNibbleY(bytecode)];
}

void op8xy1(ChipState& state, const std::uint16_t bytecode) {
    state.V[getNibbleX(bytecode)] |= state.V[getNibbleY(bytecode)];
}

void op8xy2(ChipState& state, const std::uint16_t bytecode) {
    state.V[getNibbleX(bytecode)] &= state.V[getNibbleY(bytecode)];
}

void op8xy3(ChipState& state, const std::uint16_t bytecode) {
    state.V[getNibbleX(bytecode)] ^= state.V[getNibbleY(bytecode)];
}

void op8xy4(ChipState& state, const std::uint16_t bytecode) {
    const auto kNibbleX = getNibbleX(bytecode);

    const auto kResult =
        static_cast<unsigned int>(state.V[kNibbleX]) +
        static_cast<unsigned int>(state.V[getNibbleY(bytecode)]);

    state.V[0xF] = static_cast<std::uint8_t>((kResult & 0x100U) >> kByteWidth);
    state.V[kNibbleX] = static_cast<std::uint8_t>(kResult);
}

void op8xy5(ChipState& state, const std::uint16_t bytecode) {
    const auto kNibbleX = getNibbleX(bytecode);
    const auto kNibbleY = getNibbleY(bytecode);

    // TODO: Remove branch
    state.V[0xF] = (state.V[kNibbleX] > state.V[kNibbleY]) ? 0x01U : 0x00U;

    state.V[kNibbleX] =
        static_cast<std::uint8_t>(state.V[kNibbleX] - state.V[kNibbleY]);
}

void op8xy6(ChipState& state, const std::uint16_t bytecode) {
    const auto kNibbleX = getNibbleX(bytecode);

    const auto kTemp = static_cast<unsigned int>(state.V[kNibbleX]);

    state.V[0xF] = static_cast<std::uint8_t>(kTemp & 0x1U);
    state.V[kNibbleX] >>= 1U;
}

void op8xy7(ChipState& state, const std::uint16_t bytecode) {
    const auto kNibbleX = getNibbleX(bytecode);
    const auto kNibbleY = getNibbleY(bytecode);

    // TODO: Remove branch
    state.V[0xF] = (state.V[kNibbleY] > state.V[kNibbleX]) ? 0x01U : 0x00U;

    state.V[kNibbleX] =
        static_cast<std::uint8_t>(state.V[kNibbleY] - state.V[kNibbleX]);
}

void op8xyE(ChipState& state, const std::uint16_t bytecode) {
    const auto kNibbleX = getNibbleX(bytecode);

    const auto kResult = static_cast<unsigned int>(state.V[kNibbleX]) << 1U;

    state.V[0xF] = static_cast<std::uint8_t>((kResult & 0x100U) >> kByteWidth);
    state.V[kNibbleX] = static_cast<std::uint8_t>(kResult);
}

void op9xy0(ChipState& state, const std::uint16_t bytecode) {
    if (state.V[getNibbleX(bytecode)] != state.V[getNibbleY(bytecode)]) {
        state.program_counter += 2U;
    }
}

void opAnnn(ChipState& state, const std::uint16_t bytecode) {
    state.index_register = getAddress(bytecode);
}

void opBnnn(ChipState& state, const std::uint16_t bytecode) {
    state.program_counter = getAddress(bytecode) + state.V[0];
}

void opCxkk(ChipState& state, const std::uint16_t bytecode) {
    state.V[getNibbleX(bytecode)] = state.rnd() & getAddress(bytecode);
}

void opDxyn(ChipState& state, const std::uint16_t bytecode) {
    const auto kCordX =
        static_cast<std::size_t>(state.V[getNibbleX(bytecode)]) %
        display::kWidth;
    const auto kCordY =
        static_cast<std::size_t>(state.V[getNibbleY(bytecode)]) %
        display::kHeight;

    const auto kNibbleN = static_cast<std::size_t>(getNibbleN(bytecode));

    state.V[0xF] = 0U;
    for (std::size_t j = 0; j < kNibbleN; j++) {
        if ((kCordY + j) == display::kHeight) {
            break;
        }

        const auto kSprite =
            static_cast<unsigned int>(state.memory[state.index_register + j]);
        for (std::size_t i = 0; i < kByteWidth; i++) {
            if ((kCordX + i) == display::kWidth) {
                break;
            }

            auto& old_pixel =
                state.display[(kCordX + i) + ((kCordY + j) * display::kWidth)];
            const auto kNewPixel = (kSprite >> (kByteWidth - (i + 1U))) & 0x1U;

            state.V[0xF] |= static_cast<std::uint8_t>(kNewPixel & old_pixel);
            old_pixel ^= static_cast<std::uint8_t>(kNewPixel);
        }
    }
}

void opEx9E(ChipState& state, const std::uint16_t bytecode) {
    const auto kNibbleX = getNibbleX(bytecode);

    if (state.keyboard[keyboard::mapping(kNibbleX)]) {
        state.program_counter += 2U;
    }
}

void opExA1(ChipState& state, const std::uint16_t bytecode) {
    const auto kNibbleX = getNibbleX(bytecode);

    if (!state.keyboard[keyboard::mapping(kNibbleX)]) {
        state.program_counter += 2U;
    }
}

void opFx07(ChipState& state, const std::uint16_t bytecode) {
    state.V[getNibbleX(bytecode)] = state.delay_timer;
}

void opFx0A(ChipState& state, const std::uint16_t bytecode) {
    int key_pressed = -1;
    for (auto i = 0; i < keyboard::kNumKeys; i++) {
        if (state.keyboard[keyboard::mapping(i)]) {
            key_pressed = i;
            break;
        }
    }

    if (key_pressed == -1) {
        // Run this instruction again in the next cycle
        state.program_counter -= 2U;
    } else {
        const auto kNibbleX = getNibbleX(bytecode);
        state.V[kNibbleX] = static_cast<std::uint8_t>(key_pressed);
    }
}

void opFx15(ChipState& state, const std::uint16_t bytecode) {
    state.delay_timer = state.V[getNibbleX(bytecode)];
}

void opFx18(ChipState& state, const std::uint16_t bytecode) {
    state.sound_timer = state.V[getNibbleX(bytecode)];
}

void opFx1E(ChipState& state, const std::uint16_t bytecode) {
    state.index_register += state.V[getNibbleX(bytecode)];
}

void opFx29(ChipState& state, const std::uint16_t bytecode) {
    const auto kDigit =
        static_cast<std::uint16_t>(state.V[getNibbleX(bytecode)]);

    state.index_register = font::kMemoryOffset + (kDigit * font::kSpriteSize);
}

void opFx33(ChipState& state, const std::uint16_t bytecode) {
    auto value = static_cast<unsigned int>(state.V[getNibbleX(bytecode)]);

    state.memory[state.index_register + 2] =
        static_cast<std::uint8_t>(value % 10U);
    value /= 10U;

    state.memory[state.index_register + 1] =
        static_cast<std::uint8_t>(value % 10U);
    value /= 10U;

    state.memory[state.index_register] = static_cast<std::uint8_t>(value % 10U);
}

void opFx55(ChipState& state, const std::uint16_t bytecode) {
    const auto kNibbleX = getNibbleX(bytecode);

    for (int idx = state.index_register, rgs = 0; rgs <= kNibbleX;
         idx++, rgs++) {
        state.memory[idx] = state.V[rgs];
    }
}

void opFx65(ChipState& state, const std::uint16_t bytecode) {
    const auto kNibbleX = getNibbleX(bytecode);

    for (int idx = state.index_register, rgs = 0; rgs <= kNibbleX;
         idx++, rgs++) {
        state.V[rgs] = state.memory[idx];
    }
}

}  // namespace emu::instruction_set