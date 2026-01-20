#include <iostream>
#include <memory>
#include "../include/Core/GameException.hpp"
#include "../include/Entities/Gracz.hpp"
#include "../include/Entities/Przeciwnik.hpp"
#include "../include/Items/Miecz.hpp"

using namespace RPG::Entities;
using namespace RPG::Items;
using namespace RPG::Core;

int main() {
    try {
        std::cout << "=== Start Gry RPG ===\n\n";

        Gracz bohater("Wiedzmin", 100, 10);

        Goblin potwor("Zlosliwy Goblin");

        std::cout << "--- Tura 1: Walka na piesci ---\n";
        bohater.atakuj(potwor);
        potwor.atakuj(bohater);

        std::cout << "\n";

        auto miecz = std::make_unique<Miecz>("Srebrny Miecz", 20);

        bohater.zalozBron(std::move(miecz));

        std::cout << "\n--- Tura 2: Walka mieczem ---\n";
        bohater.atakuj(potwor);

        if (!potwor.czyZyje()) {
            std::cout << "\nPotwor zostal pokonany!\n";
        }
    
    }

    catch (const GameException& e) {
        std::cerr << "\n[BLAD GRY]: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "\n[BLAD SYSTEMOWY]: " << e.what() << std::endl;
    }

    return 0;
}