#ifndef CHIP_8_ERROR_HANDLING_HPP
#define CHIP_8_ERROR_HANDLING_HPP

#include <stdexcept>

namespace emu {

class InvalidInstructionError : public std::runtime_error {
   public:
    explicit InvalidInstructionError()
        : std::runtime_error("Invalid instruction") {};
    explicit InvalidInstructionError(const std::string& message)
        : std::runtime_error(message) {}
};

}  // namespace emu

#endif /* CHIP_8_ERROR_HANDLING_HPP */
