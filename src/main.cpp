#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stack>

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "SDL3/SDL_init.h"

// RAM memory
std::array<std::uint8_t, 4096> memory;
// Stack
std::stack<std::uint16_t> stack;
// Program counter
std::uint16_t program_counter;
// Index register
std::uint16_t index_register;
// Registers
std::array<std::uint8_t, 16> V;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    // Open test ROM
    std::ifstream file("test/test-opcode.ch8",
                       std::ifstream::ate | std::ifstream::binary);
    if (!file.is_open()) {
        SDL_Log("Failed to open file");
        return SDL_APP_FAILURE;
    }

    // Get size of file
    std::streamsize size = file.tellg();
    // Set file at beggining
    file.seekg(std::ifstream::beg);

    // Load test ROM into memory at 0x00
    if (!file.read(reinterpret_cast<char*>(memory.data()), size)) {
        SDL_Log("Failed to read file");
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS; /* end the program, reporting success to the OS.
                                 */
    }
    return SDL_APP_CONTINUE;
}

//////////////////////////////// Instructions ////////////////////////////////
// NOLINTBEGIN (misc-use-internal-linkage)

using Instruction = void (*)(const std::uint16_t);

/**
 * @brief JMP to a host machine code - Treated as NOP
 *
 * @param instruction
 */
void op0nnn(const std::uint16_t /* not used */) {}

/**
 * @brief CLS - Clear the display.
 *
 * @param instruction
 */
void op00E0(const std::uint16_t instruction) {
    // TODO
}

/**
 * @brief Return Call - The interpreter sets the program counter to the address
 * at the top of the stack, then subtracts 1 from the stack pointer.
 *
 * @param instruction
 */
void op00EE(const std::uint16_t instruction) {
    if (stack.empty()) {
        throw std::runtime_error("Stack underflow on RET");
    }

    program_counter = stack.top();
    stack.pop();
}

/**
 * @brief JMP to address - The interpreter sets the program counter to nnn.
 *
 * @param instruction
 */
void op1nnn(const std::uint16_t instruction) {
    program_counter = instruction & 0x0FFF;
}

/**
 * @brief Call address - The interpreter increments the stack pointer, then puts
 * the current PC on the top of the stack. The PC is then set to nnn.
 *
 * @param instruction
 */
void op2nnn(const std::uint16_t instruction) {
    stack.push(program_counter);
    program_counter = instruction & 0x0FFF;
}

/**
 * @brief SE Vx, byte - Skip next instruction if Vx = kk.
 *
 * @param instruction
 */
void op3xkk(const std::uint16_t instruction) {
    if (V[(instruction & 0x0F00) >> 8] == (instruction & 0x00FF)) {
        program_counter += 2;
    }
}

/**
 * @brief SNE Vx, byte - Skip next instruction if Vx != kk.
 *
 * @param instruction
 */
void op4xkk(const std::uint16_t instruction) {
    if (V[(instruction & 0x0F00) >> 8] != (instruction & 0x00FF)) {
        program_counter += 2;
    }
}

/**
 * @brief SE Vx, Vy - Skip next instruction if Vx = Vy.
 *
 * @param instruction
 */
void op5xy0(const std::uint16_t instruction) {
    if (V[(instruction & 0x0F00) >> 8] == V[(instruction & 0x00F0) >> 4]) {
        program_counter += 2;
    }
}

/**
 * @brief LD Vx, byte - Set Vx = kk.
 *
 * @param instruction
 */
void op6xkk(const std::uint16_t instruction) {
    V[(instruction & 0x0F00) >> 8] = instruction & 0x00FF;
}

/**
 * @brief ADD Vx, byte - Set Vx = Vx + kk.
 *
 * @param instruction
 */
void op7xkk(const std::uint16_t instruction) {
    V[(instruction & 0x0F00) >> 8] += instruction & 0x00FF;
}

/**
 * @brief LD Vx, Vy - Set Vx = Vy.
 *
 * @param instruction
 */
void op8xy0(const std::uint16_t instruction) {
    V[(instruction & 0x0F00) >> 8] = V[(instruction & 0x00F0) >> 4];
}

/**
 * @brief OR Vx, Vy - Set Vx = Vx OR Vy.
 *
 * @param instruction
 */
void op8xy1(const std::uint16_t instruction) {
    V[(instruction & 0x0F00) >> 8] |= V[(instruction & 0x00F0) >> 4];
}

/**
 * @brief AND Vx, Vy - Set Vx = Vx AND Vy.
 *
 * @param instruction
 */
void op8xy2(const std::uint16_t instruction) {
    V[(instruction & 0x0F00) >> 8] &= V[(instruction & 0x00F0) >> 4];
}

/**
 * @brief XOR Vx, Vy - Set Vx = Vx XOR Vy.
 *
 * @param instruction
 */
void op8xy3(const std::uint16_t instruction) {
    V[(instruction & 0x0F00) >> 8] ^= V[(instruction & 0x00F0) >> 4];
}

/**
 * @brief ADD Vx, Vy - Set Vx = Vx + Vy, set VF = carry.
 *
 * @param instruction
 */
void op8xy4(const std::uint16_t instruction) {}

/**
 * @brief SUB Vx, Vy - Set Vx = Vx - Vy, set VF = NOT borrow.
 *
 * @param instruction
 */
void op8xy5(const std::uint16_t instruction) {}

/**
 * @brief SHR Vx {, Vy} - Set Vx = Vx SHR 1.
 * @todo Then why y is there?
 * @param instruction
 */
void op8xy6(const std::uint16_t instruction) {}

/**
 * @brief SUBN Vx, Vy - Set Vx = Vy - Vx, set VF = NOT borrow.
 *
 * @param instruction
 */
void op8xy7(const std::uint16_t instruction) {}

/**
 * @brief SHL Vx {, Vy} - Set Vx = Vx SHL 1.
 * @todo Then why y is there?
 * @param instruction
 */
void op8xyE(const std::uint16_t instruction) {}

/**
 * @brief SNE Vx, Vy - Skip next instruction if Vx != Vy.
 *
 * @param instruction
 */
void op9xy0(const std::uint16_t instruction) {
    if (V[(instruction & 0x0F00) >> 8] != V[(instruction & 0x00F0) >> 4]) {
        program_counter += 2;
    }
}

/**
 * @brief LD I, addr - Set I = nnn.
 *
 * @param instruction
 */
void opAnnn(const std::uint16_t instruction) {
    index_register = instruction & 0x0FFF;
}

/**
 * @brief JP V0, addr - Jump to location nnn + V0.
 *
 * @param instruction
 */
void opBnnn(const std::uint16_t instruction) {
    program_counter = (instruction & 0x0FFF) + V[0];
}

/**
 * @brief RND Vx, byte - Set Vx = random byte AND kk.
 *
 * @param instruction
 */
void opCxkk(const std::uint16_t instruction) {}

/**
 * @brief DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location
 * I at (Vx, Vy), set VF = collision.
 *
 * @param instruction
 */
void opDxyn(const std::uint16_t instruction) {}

/**
 * @brief SKP Vx - Skip next instruction if key with the value of Vx is pressed.
 *
 * @param instruction
 */
void opEx9E(const std::uint16_t instruction) {}

/**
 * @brief SKNP Vx - Skip next instruction if key with the value of Vx is not
 * pressed.
 *
 * @param instruction
 */
void opExA1(const std::uint16_t instruction) {}

/**
 * @brief LD Vx, DT - Set Vx = delay timer value.
 *
 * @param instruction
 */
void opFx07(const std::uint16_t instruction) {}

/**
 * @brief LD Vx, K - Wait for a key press, store the value of the key in Vx.
 *
 * @param instruction
 */
void opFx0A(const std::uint16_t instruction) {}

/**
 * @brief LD DT, Vx - Set delay timer = Vx.
 *
 * @param instruction
 */
void opFx15(const std::uint16_t instruction) {}

/**
 * @brief LD ST, Vx - Set sound timer = Vx.
 *
 * @param instruction
 */
void opFx18(const std::uint16_t instruction) {}

/**
 * @brief ADD I, Vx - Set I = I + Vx.
 *
 * @param instruction
 */
void opFx1E(const std::uint16_t instruction) {}

/**
 * @brief LD F, Vx - Set I = location of sprite for digit Vx.
 *
 * @param instruction
 */
void opFx29(const std::uint16_t instruction) {}

/**
 * @brief LD B, Vx - Store BCD representation of Vx in memory locations I, I+1,
 * and I+2.
 *
 * @param instruction
 */
void opFx33(const std::uint16_t instruction) {}

/**
 * @brief LD [I], Vx - Store registers V0 through Vx in memory starting at
 * location I.
 *
 * @param instruction
 */
void opFx55(const std::uint16_t instruction) {}

/**
 * @brief Fx65 - LD Vx, [I] - Read registers V0 through Vx from memory starting
 * at location I.
 *
 * @param instruction
 */
void opFx65(const std::uint16_t instruction) {}

// NOLINTEND (misc-use-internal-linkage)
//////////////////////////////// Instructions ////////////////////////////////

//////////////////////////////// Fetch ////////////////////////////////

std::uint16_t fetch() {
    const auto kInstruction = static_cast<std::uint16_t>(
        memory[program_counter] << 8U | memory[program_counter + 1]);
    program_counter += 2;

    return kInstruction;
}

//////////////////////////////// Fetch ////////////////////////////////

//////////////////////////////// Decoder ////////////////////////////////

Instruction handleGroup0(const std::uint16_t instruction) {
    switch (instruction & 0x0FFF) {
        case 0x00E0:
            return op00E0;
        case 0x00EE:
            return op00EE;
        default:
            return op0nnn;
    }
}

Instruction handleGroup1(const std::uint16_t instruction) {
    return op1nnn;
}

Instruction handleGroup2(const std::uint16_t instruction) {
    return op2nnn;
}

Instruction handleGroup3(const std::uint16_t instruction) {
    return op3xkk;
}

Instruction handleGroup4(const std::uint16_t instruction) {
    return op4xkk;
}

Instruction handleGroup5(const std::uint16_t instruction) {
    return op5xy0;
}

Instruction handleGroup6(const std::uint16_t instruction) {
    return op6xkk;
}

Instruction handleGroup7(const std::uint16_t instruction) {
    return op7xkk;
}

Instruction handleGroup8(const std::uint16_t instruction) {
    switch (instruction & 0x000F) {
        case 0x0000:
            return op8xy0;
        case 0x0001:
            return op8xy1;
        case 0x0002:
            return op8xy2;
        case 0x0003:
            return op8xy3;
        case 0x0004:
            return op8xy4;
        case 0x0005:
            return op8xy5;
        case 0x0006:
            return op8xy6;
        case 0x0007:
            return op8xy7;
        case 0x000E:
            return op8xyE;
        default:
            break;
    }
}

Instruction handleGroup9(const std::uint16_t instruction) {
    return op9xy0;
}

Instruction handleGroupA(const std::uint16_t instruction) {
    return opAnnn;
}

Instruction handleGroupB(const std::uint16_t instruction) {
    return opBnnn;
}

Instruction handleGroupC(const std::uint16_t instruction) {
    return opCxkk;
}

Instruction handleGroupD(const std::uint16_t instruction) {
    return opDxyn;
}

Instruction handleGroupE(const std::uint16_t instruction) {
    switch (instruction & 0x00FF) {
        case 0x009E:
            return opEx9E;
        case 0x00A1:
            return opExA1;
        default:
            break;
    }
}

Instruction handleGroupF(const std::uint16_t instruction) {
    switch (instruction & 0x00FF) {
        case 0x0007:
            return opFx07;
        case 0x000A:
            return opFx0A;
        case 0x0015:
            return opFx15;
        case 0x0018:
            return opFx18;
        case 0x001E:
            return opFx1E;
        case 0x0029:
            return opFx29;
        case 0x0033:
            return opFx33;
        case 0x0055:
            return opFx55;
        case 0x0065:
            return opFx65;
        default:
            break;
    }
}

Instruction decode(const std::uint16_t instruction) {
    switch (instruction & 0xF000U) {
        case 0x0000:
            return handleGroup0(instruction);
        case 0x1000:
            return handleGroup1(instruction);
        case 0x2000:
            return handleGroup2(instruction);
        case 0x3000:
            return handleGroup3(instruction);
        case 0x4000:
            return handleGroup4(instruction);
        case 0x5000:
            return handleGroup5(instruction);
        case 0x6000:
            return handleGroup6(instruction);
        case 0x7000:
            return handleGroup7(instruction);
        case 0x8000:
            return handleGroup8(instruction);
        case 0x9000:
            return handleGroup9(instruction);
        case 0xA000:
            return handleGroupA(instruction);
        case 0xB000:
            return handleGroupB(instruction);
        case 0xC000:
            return handleGroupC(instruction);
        case 0xD000:
            return handleGroupD(instruction);
        case 0xE000:
            return handleGroupE(instruction);
        case 0xF000:
            return handleGroupF(instruction);
        default:
            break;
    }
}

//////////////////////////////// Decoder ////////////////////////////////

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate) {
    // Fetch an instruction
    const auto kInstruction = fetch();

    if (kInstruction == 0x0000 || program_counter >= memory.size()) {
        return SDL_APP_SUCCESS;
    }

    // Decode instruction
    const auto kDecodedInstruction = decode(kInstruction);

    // Execute decoded instruction
    kDecodedInstruction(kInstruction);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result) {}
