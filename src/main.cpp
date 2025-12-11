#include <fstream>
#include <iostream>

#include "chip_8/chip_8.hpp"

#define SDL_MAIN_USE_CALLBACKS 1
#include "SDL3/SDL.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_main.h"

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

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate) {
    // // Fetch an instruction
    // const auto kInstruction = fetch();

    // if (kInstruction == 0x0000 || program_counter >= memory.size()) {
    //     return SDL_APP_SUCCESS;
    // }

    // // Decode instruction
    // const auto kDecodedInstruction = decode(kInstruction);

    // // Execute decoded instruction
    // kDecodedInstruction(kInstruction);

    // return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result) {}
