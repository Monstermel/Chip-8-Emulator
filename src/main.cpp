#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stack>
#include "SDL3/SDL_init.h"

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// RAM memory
std::array<std::uint8_t, 4096> memory;
// Stack
std::stack<std::uint16_t, std::array<std::uint16_t, 16>> stack;
// Program counter
std::uint16_t program_counter;
// Index register
std::uint16_t index_register;
// Registers
std::uint8_t V0;
std::uint8_t V1;
std::uint8_t V2;
std::uint8_t V3;
std::uint8_t V4;
std::uint8_t V5;
std::uint8_t V6;
std::uint8_t V7;
std::uint8_t V8;
std::uint8_t V9;
std::uint8_t VA;
std::uint8_t VB;
std::uint8_t VC;
std::uint8_t VD;
std::uint8_t VE;
std::uint8_t VF;

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

std::uint16_t fetch() {
    const auto kInstruction = static_cast<std::uint16_t>(
        memory[program_counter] << 8U | memory[program_counter + 1]);
    program_counter += 2;

    return kInstruction;
}

void handleGroup0(const std::uint16_t instruction) {
    switch (instruction & 0x0FFF) {
        case 0x0E0:
            std::cout << "CLS\n";
        case 0x0EE:
            std::cout << "RET\n";
        default:
            std::cout << "SYS addr\n";
    }
}

void handleGroup1(const std::uint16_t instruction) {
    std::cout << "JP addr\n";
}

void handleGroup2(const std::uint16_t instruction) {
    std::cout << "CALL addr\n";
}

void handleGroup3(const std::uint16_t instruction) {
    std::cout << "SE Vx, byte\n";
}

void handleGroup4(const std::uint16_t instruction) {
    std::cout << "SNE Vx, byte\n";
}

void handleGroup5(const std::uint16_t instruction) {
    std::cout << "SE Vx, Vy\n";
}

void handleGroup6(const std::uint16_t instruction) {
    std::cout << "LD Vx, byte\n";
}

void handleGroup7(const std::uint16_t instruction) {
    std::cout << "ADD Vx, byte\n";
}

void handleGroup8(const std::uint16_t instruction) {
    switch (instruction & 0x000F) {
        case 0x0000:
            std::cout << "LD Vx, Vy\n";
            break;
        case 0x0001:
            std::cout << "OR Vx, Vy\n";
            break;
        case 0x0002:
            std::cout << "AND Vx, Vy\n";
            break;
        case 0x0003:
            std::cout << "XOR Vx, Vy\n";
            break;
        case 0x0004:
            std::cout << "ADD Vx, Vy\n";
            break;
        case 0x0005:
            std::cout << "SUB Vx, Vy\n";
            break;
        case 0x0006:
            std::cout << "SHR Vx {, Vy}\n";
            break;
        case 0x0007:
            std::cout << "SUBN Vx, Vy\n";
            break;
        case 0x000E:
            std::cout << "SHL Vx {, Vy}\n";
            break;
        default:
            break;
    }
}

void handleGroup9(const std::uint16_t instruction) {
    std::cout << "SNE Vx, Vy\n";
}

void handleGroupA(const std::uint16_t instruction) {
    std::cout << "LD I, addr\n";
}

void handleGroupB(const std::uint16_t instruction) {
    std::cout << "JP V0, addr\n";
}

void handleGroupC(const std::uint16_t instruction) {
    std::cout << "RND Vx, byte\n";
}

void handleGroupD(const std::uint16_t instruction) {
    std::cout << "DRW Vx, Vy, nibble\n";
}

void handleGroupE(const std::uint16_t instruction) {
    switch (instruction & 0x00FF) {
        case 0x009E:
            std::cout << "SKP Vx\n";
            break;
        case 0x00A1:
            std::cout << "SKNP Vx\n";
            break;
        default:
            break;
    }
}

void handleGroupF(const std::uint16_t instruction) {
    switch (instruction & 0x00FF) {
        case 0x0007:
            std::cout << "LD Vx, DT\n";
            break;
        case 0x000A:
            std::cout << "LD Vx, K\n";
            break;
        case 0x0015:
            std::cout << "LD DT, Vx\n";
            break;
        case 0x0018:
            std::cout << "LD ST, Vx\n";
            break;
        case 0x001E:
            std::cout << "ADD I, Vx\n";
            break;
        case 0x0029:
            std::cout << "LD F, Vx\n";
            break;
        case 0x0033:
            std::cout << "LD B, Vx\n";
            break;
        case 0x0055:
            std::cout << "LD [I], Vx\n";
            break;
        case 0x0065:
            std::cout << "LD Vx, [I]\n";
            break;
        default:
            break;
    }
}

void decode(const std::uint16_t instruction) {
    switch (instruction & 0xF000U) {
        case 0x0000:
            handleGroup0(instruction);
            break;
        case 0x1000:
            handleGroup1(instruction);
            break;
        case 0x2000:
            handleGroup2(instruction);
            break;
        case 0x3000:
            handleGroup3(instruction);
            break;
        case 0x4000:
            handleGroup4(instruction);
            break;
        case 0x5000:
            handleGroup5(instruction);
            break;
        case 0x6000:
            handleGroup6(instruction);
            break;
        case 0x7000:
            handleGroup7(instruction);
            break;
        case 0x8000:
            handleGroup8(instruction);
            break;
        case 0x9000:
            handleGroup9(instruction);
            break;
        case 0xA000:
            handleGroupA(instruction);
            break;
        case 0xB000:
            handleGroupB(instruction);
            break;
        case 0xC000:
            handleGroupC(instruction);
            break;
        case 0xD000:
            handleGroupD(instruction);
            break;
        case 0xE000:
            handleGroupE(instruction);
            break;
        case 0xF000:
            handleGroupF(instruction);
            break;
        default:
            break;
    }
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate) {
    // Fetch an instruction
    const auto kInstruction = fetch();

    // If null instruction, finish program loop
    if (kInstruction == 0x0000) {
        return SDL_APP_SUCCESS;
    }

    // Decode instruction
    decode(kInstruction);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result) {}
