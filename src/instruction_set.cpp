#include "chip_8/instruction_set.hpp"

#include "chip_8/error.hpp"
#include "chip_8/utility.hpp"

namespace emu::instruction_set {

void op0nnn(ChipState& /* not used */, const std::uint16_t /* not used */) {}

void op00E0(ChipState& state, const std::uint16_t bytecode) {
    // TODO
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

void op8xy4(ChipState& state, const std::uint16_t bytecode) {}

void op8xy5(ChipState& state, const std::uint16_t bytecode) {}

void op8xy6(ChipState& state, const std::uint16_t bytecode) {}

void op8xy7(ChipState& state, const std::uint16_t bytecode) {}

void op8xyE(ChipState& state, const std::uint16_t bytecode) {}

void op9xy0(ChipState& state, const std::uint16_t bytecode) {
    if (state.V[getNibbleX(bytecode)] != state.V[getNibbleY(bytecode)]) {
        state.program_counter += 2;
    }
}

void opAnnn(ChipState& state, const std::uint16_t bytecode) {
    state.index_register = getAddress(bytecode);
}

void opBnnn(ChipState& state, const std::uint16_t bytecode) {
    state.program_counter = getAddress(bytecode) + state.V[0];
}

void opCxkk(ChipState& state, const std::uint16_t bytecode) {}

void opDxyn(ChipState& state, const std::uint16_t bytecode) {}

void opEx9E(ChipState& state, const std::uint16_t bytecode) {}

void opExA1(ChipState& state, const std::uint16_t bytecode) {}

void opFx07(ChipState& state, const std::uint16_t bytecode) {
    state.V[getNibbleX(bytecode)] = state.delay_timer;
}

void opFx0A(ChipState& state, const std::uint16_t bytecode) {}

void opFx15(ChipState& state, const std::uint16_t bytecode) {
    state.delay_timer = state.V[getNibbleX(bytecode)];
}

void opFx18(ChipState& state, const std::uint16_t bytecode) {
    state.sound_timer = state.V[getNibbleX(bytecode)];
}

void opFx1E(ChipState& state, const std::uint16_t bytecode) {
    state.index_register += state.V[getNibbleX(bytecode)];
}

void opFx29(ChipState& state, const std::uint16_t bytecode) {}

void opFx33(ChipState& state, const std::uint16_t bytecode) {}

void opFx55(ChipState& state, const std::uint16_t bytecode) {}

void opFx65(ChipState& state, const std::uint16_t bytecode) {}

}  // namespace emu::instruction_set