#include "chip_8/instruction_set.hpp"

namespace emu::instruction_set {

void op0nnn(ChipState& /* not used */, const std::uint16_t /* not used */) {}

void op00E0(ChipState& state, const std::uint16_t bytecode) {
    // TODO
}

void op00EE(ChipState& state, const std::uint16_t /* not used */) {
    if (state.stack.empty()) {
        throw std::runtime_error("Stack underflow on RET");
    }

    state.program_counter = state.stack.top();
    state.stack.pop();
}

void op1nnn(ChipState& state, const std::uint16_t bytecode) {
    state.program_counter = bytecode & 0x0FFF;
}

void op2nnn(ChipState& state, const std::uint16_t bytecode) {
    state.stack.push(state.program_counter);
    state.program_counter = bytecode & 0x0FFF;
}

void op3xkk(ChipState& state, const std::uint16_t bytecode) {
    if (state.V[(bytecode & 0x0F00) >> 8] == (bytecode & 0x00FF)) {
        state.program_counter += 2;
    }
}

void op4xkk(ChipState& state, const std::uint16_t bytecode) {
    if (state.V[(bytecode & 0x0F00) >> 8] != (bytecode & 0x00FF)) {
        state.program_counter += 2;
    }
}

void op5xy0(ChipState& state, const std::uint16_t bytecode) {
    if (state.V[(bytecode & 0x0F00) >> 8] ==
        state.V[(bytecode & 0x00F0) >> 4]) {
        state.program_counter += 2;
    }
}

void op6xkk(ChipState& state, const std::uint16_t bytecode) {
    state.V[(bytecode & 0x0F00) >> 8] = bytecode & 0x00FF;
}

void op7xkk(ChipState& state, const std::uint16_t bytecode) {
    state.V[(bytecode & 0x0F00) >> 8] += bytecode & 0x00FF;
}

void op8xy0(ChipState& state, const std::uint16_t bytecode) {
    state.V[(bytecode & 0x0F00) >> 8] = state.V[(bytecode & 0x00F0) >> 4];
}

void op8xy1(ChipState& state, const std::uint16_t bytecode) {
    state.V[(bytecode & 0x0F00) >> 8] |= state.V[(bytecode & 0x00F0) >> 4];
}

void op8xy2(ChipState& state, const std::uint16_t bytecode) {
    state.V[(bytecode & 0x0F00) >> 8] &= state.V[(bytecode & 0x00F0) >> 4];
}

void op8xy3(ChipState& state, const std::uint16_t bytecode) {
    state.V[(bytecode & 0x0F00) >> 8] ^= state.V[(bytecode & 0x00F0) >> 4];
}

void op8xy4(ChipState& state, const std::uint16_t bytecode) {}

void op8xy5(ChipState& state, const std::uint16_t bytecode) {}

void op8xy6(ChipState& state, const std::uint16_t bytecode) {}

void op8xy7(ChipState& state, const std::uint16_t bytecode) {}

void op8xyE(ChipState& state, const std::uint16_t bytecode) {}

void op9xy0(ChipState& state, const std::uint16_t bytecode) {
    if (state.V[(bytecode & 0x0F00) >> 8] !=
        state.V[(bytecode & 0x00F0) >> 4]) {
        state.program_counter += 2;
    }
}

void opAnnn(ChipState& state, const std::uint16_t bytecode) {
    state.index_register = bytecode & 0x0FFF;
}

void opBnnn(ChipState& state, const std::uint16_t bytecode) {
    state.program_counter = (bytecode & 0x0FFF) + state.V[0];
}

void opCxkk(ChipState& state, const std::uint16_t bytecode) {}

void opDxyn(ChipState& state, const std::uint16_t bytecode) {}

void opEx9E(ChipState& state, const std::uint16_t bytecode) {}

void opExA1(ChipState& state, const std::uint16_t bytecode) {}

void opFx07(ChipState& state, const std::uint16_t bytecode) {
    state.V[(bytecode & 0x0F00) >> 8] = state.delay_timer;
}

void opFx0A(ChipState& state, const std::uint16_t bytecode) {}

void opFx15(ChipState& state, const std::uint16_t bytecode) {
    state.delay_timer = state.V[(bytecode & 0x0F00) >> 8];
}

void opFx18(ChipState& state, const std::uint16_t bytecode) {
    state.sound_timer = state.V[(bytecode & 0x0F00) >> 8];
}

void opFx1E(ChipState& state, const std::uint16_t bytecode) {
    state.index_register += state.V[(bytecode & 0x0F00) >> 8];
}

void opFx29(ChipState& state, const std::uint16_t bytecode) {}

void opFx33(ChipState& state, const std::uint16_t bytecode) {}

void opFx55(ChipState& state, const std::uint16_t bytecode) {}

void opFx65(ChipState& state, const std::uint16_t bytecode) {}

}  // namespace emu::instruction_set