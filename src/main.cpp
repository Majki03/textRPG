#include <iostream>
#include "../include/Core/Game.hpp"
#include "../include/Core/GameException.hpp"

int main() {
    try {
        RPG::Core::Game gra;

        gra.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Krytyczny blad gry: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}