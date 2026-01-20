#include "../../include/Core/Game.hpp"
#include "../../include/Entities/Przeciwnik.hpp"
#include "../../include/Items/Miecz.hpp"
#include <iostream>
#include <random>
#include <ctime>

namespace RPG::Core {

    Game::Game() : isRunning(true) {
        std::cout << "Podaj imie bohatera: ";
        std::string imie;
        std::cin >> imie;

        gracz = std::make_unique<RPG::Entities::Gracz>(imie, 100, 5);
    }

    void Game::run() {
        std::cout << "\nWitaj w świecie Katalon, " << gracz->getImie() << "!\n";

        while (isRunning && gracz->czyZyje()) {
            menuGlowne();
        }

        if (!gracz->czyZyje()) {
            std::cout << "\n=== GAME OVER ===\nTwoj bohater polegl.\n";
        } else {
            std::cout << "\nDo zobaczenia!\n";
        }
    }

    void Game::menuGlowne() {
        std::cout << "\n--------------------------\n";
        std::cout << "Co chcesz zrobic?\n";
        std::cout << "1. Eksploruj swiat (Szukaj walki)\n";
        std::cout << "2. Odpocznij (Leczenie)\n";
        std::cout << "3. Pokaz statystyki\n";
        std::cout << "4. Wyjdz z gry\n";
        std::cout << "Wybor: ";

        int wybor;
        std::cin >> wybor;

        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Nieprawidlowa opcja.\n";
            return;
        }

        switch (wybor) {
            case 1: eksploruj(); break;
            case 2:
                gracz->lecz(20);
                break;
            case 3:
                std::cout << "\n[STATYSTYKI] " << gracz->getImie()
                            << " HP: " << gracz->getHp() << "/" << gracz->getMaxHp() << "\n";
                break;
            case 4: isRunning = false; break;
            default: std::cout << "Nieznana opcja.\n";
        }
    }

    void Game::eksploruj() {
        std::cout << "\nWedrujesz przez mroczny las...\n";

        int los = losujLiczbe(1, 100);

        if (los <= 30) {
            std::cout << "Znalazles stary, zardzewialy miecz!\n";
            auto miecz = std::make_unique<RPG::Items::Miecz>("Zardzewialy Miecz", 15);
            gracz->zalozBron(std::move(miecz));
        } else {
            walka();
        }
    }

    void Game::walka() {
        auto przeciwnik = std::make_unique<RPG::Entities::Goblin>("Zlosliwy Goblin");

        std::cout << "\n!!! ATAKUJE CIE " << przeciwnik->getImie() << " !!!\n";

        while (przeciwnik->czyZyje() && gracz->czyZyje()) {
            std::cout << "\n1. Atakuj 2. Uciekaj\nWybor: ";
            int akcja;
            std::cin >> akcja;

            if (akcja == 1) {
                gracz->atakuj(*przeciwnik);

                if (przeciwnik->czyZyje()) {
                    przeciwnik->atakuj(*gracz);
                }
            } else if (akcja == 2) {
                std::cout << "Uciekasz w poplochu!\n";
                return;
            }
        }

        if (gracz->czyZyje()) {
            std::cout << "\nZwyciestwo! Pokonales przeciwnika.\n";
            gracz->awansuj();
        }
    }

    int Game::losujLiczbe(int min, int max) {
        static std::mt19937 generator(std::time(nullptr));
        std::uniform_int_distribution<int> dystrybucja(min, max);
        return dystrybucja(generator);
    }
    
}