#include "../../include/Core/Game.hpp"
#include "../../include/Entities/Przeciwnik.hpp"
#include "../../include/Items/Miecz.hpp"
#include "../../include/Items/Mikstura.hpp"
#include "../../include/Items/Przedmiot.hpp"
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
        std::cout << "3. Ekwipunek (Zmien bron)\n";
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
            case 2: gracz->lecz(20); break;
            case 3:
                gracz->pokazEkwipunek();
                std::cout << "Wybierz numer broni do zalozenia (-1 aby anulowac): ";
                int nr;
                std::cin >> nr;
                if (nr >= 0) {
                    gracz->uzyjPrzedmiotu(nr);
                }
                break;
            case 4: isRunning = false; break;
            default: std::cout << "Nieznana opcja.\n";
        }
    }

    // ... include Mikstura, Wilk, Ork, Smok, Bandyta ...

    // Funkcja pomocnicza do generowania losowej broni
    std::unique_ptr<RPG::Items::Bron> generujBron(int poziomTrudnosci) {
        // poziomTrudnosci 1-3
        int los = rand() % 3; // Proste losowanie typu
        std::string nazwa;
        int dmg = 5 * poziomTrudnosci;

        if (los == 0) { nazwa = "Topor"; dmg += 5; }
        else if (los == 1) { nazwa = "Wlocznia"; dmg += 2; }
        else { nazwa = "Mlot"; dmg += 8; }
        
        nazwa += " (Tier " + std::to_string(poziomTrudnosci) + ")";
        return std::make_unique<RPG::Items::Bron>(nazwa, dmg);
    }

    void Game::eksploruj() {
        std::cout << "\nEksplorujesz swiat...\n";
        int los = losujLiczbe(1, 100);

        // --- TABELA ZDARZEŃ ---
        // 1-10: Znalezienie mikstury (10%)
        // 11-20: Znalezienie broni (10%)
        // 21-100: Walka (80%)
        
        if (los <= 10) {
            std::cout << "Znalazles miksture!\n";
            gracz->podniesPrzedmiot(std::make_unique<RPG::Items::Mikstura>(30));
        }
        else if (los <= 20) {
            std::cout << "Znalazles porzucona bron!\n";
            gracz->podniesPrzedmiot(generujBron(1));
        }
        else {
            walka();
        }
    }

    void Game::walka() {
        std::unique_ptr<RPG::Entities::Przeciwnik> przeciwnik;
        
        int los = losujLiczbe(1, 100);

        // --- TABELA PRZECIWNIKÓW ---
        // 1-50: Wilk (50%)
        // 51-75: Bandyta (25%)
        // 76-95: Ork (20%)
        // 96-100: Smok (5%) - rzadki BOSS
        
        if (los <= 50) {
            przeciwnik = std::make_unique<RPG::Entities::Wilk>();
        } else if (los <= 75) {
            // Bandyta ma 50% szans na posiadanie broni
            std::unique_ptr<RPG::Items::Bron> bronBandyty = nullptr;
            if (losujLiczbe(0, 1) == 1) {
                bronBandyty = generujBron(2); // Bandyta ma lepszą broń (Tier 2)
            }
            przeciwnik = std::make_unique<RPG::Entities::Bandyta>(std::move(bronBandyty));
        } else if (los <= 95) {
            przeciwnik = std::make_unique<RPG::Entities::Ork>();
        } else {
            std::cout << "\n!!! CZUJESZ ZAPACH SIARKI... POJAWIA SIE SMOK !!!\n";
            przeciwnik = std::make_unique<RPG::Entities::Smok>();
        }

        std::cout << "Walczysz z: " << przeciwnik->getImie() << "\n";

        // Pętla walki (bez zmian, tylko dodajemy EXP na końcu)
        while (przeciwnik->czyZyje() && gracz->czyZyje()) {
            // ... (logika walki taka sama jak wcześniej) ...
            // Pamiętaj, aby zaimplementować tu logikę: 
            // 1. Atak Gracza
            // 2. Jeśli przeciwnik żyje -> Atak Przeciwnika
             std::cout << "\n1. Atakuj  2. Uciekaj\nWybor: ";
            int akcja;
            std::cin >> akcja;

            if (akcja == 1) {
                gracz->atakuj(*przeciwnik);
                if (przeciwnik->czyZyje()) {
                    przeciwnik->atakuj(*gracz);
                }
            } else {
                return;
            }
        }

        if (gracz->czyZyje()) {
            std::cout << "Zwyciestwo!\n";
            
            // 1. Przyznanie EXP
            gracz->dodajExp(przeciwnik->getExpDrop());
            
            // 2. Sprawdzenie czy przeciwnik coś upuścił (Bandyta)
            auto lup = przeciwnik->upuscBron();
            if (lup) {
                std::cout << "Przeciwnik upuscil bron!\n";
                gracz->podniesPrzedmiot(std::move(lup));
            }
        }
    }

    int Game::losujLiczbe(int min, int max) {
        static std::mt19937 generator(std::time(nullptr));
        std::uniform_int_distribution<int> dystrybucja(min, max);
        return dystrybucja(generator);
    }
    
}