#pragma once
#include <stdexcept>
#include <string>

namespace RPF::Core {
    
    class GameException : public std::runtime_error {
    public:
        explicit GameException(const std::string& message)
            : std::runtime_error("RPG Error: " + message) {}
    };
    
}