#include "chip_8/chip_8.hpp"

#include <fstream>
#include <ios>
#include <iterator>

namespace emu {

int Chip8::load() {
    // Open test ROM
    std::ifstream file("test/test-opcode.ch8",
                       // NOLINTNEXTLINE (hicpp-signed-bitwise)
                       std::ifstream::ate | std::ifstream::binary);
    if (!file.is_open()) {
        return -1;
    }

    // Get size of file
    std::streamsize size = file.tellg();
    // Set file at beggining
    file.seekg(std::ifstream::beg);

    // Load test ROM into memory at kProgramSpaceOffset
    if (!file.read(std::next(reinterpret_cast<char*>(state_.memory.data()),
                             kProgramSpaceOffset),
                   size)) {
        return -1;
    }

    return 0;
}

}  // namespace emu