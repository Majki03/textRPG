#include "../../include/Core/Game.hpp"
#include "../../include/Entities/Przeciwnik.hpp"
#include "../../include/Items/Miecz.hpp"
#include "../../include/Items/Mikstura.hpp"
#include "../../include/Items/Przedmiot.hpp"
#include "../../include/Items/LegendarnaBron.hpp"
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

    std::unique_ptr<RPG::Items::Bron> generujBron(int poziomTrudnosci) {
        int los = rand() % 3;
        std::string nazwa;
        int dmg = 5 * poziomTrudnosci;

        if (los == 0) { nazwa = "Topor"; dmg += 5; }
        else if (los == 1) { nazwa = "Wlocznia"; dmg += 2; }
        else { nazwa = "Mlot"; dmg += 8; }
        
        nazwa += " (Tier " + std::to_string(poziomTrudnosci) + ")";
        return std::make_unique<RPG::Items::Bron>(nazwa, dmg);
    }

    void Game::eksploruj() {
        std::cout << "\nEksplorujesz nieznane tereny...\n";
        int los = losujLiczbe(1, 100);

        if (los <= 5) {
            std::cout << "\n!!! NIEWIARYGODNE !!!\n";
            std::cout << "Zauwazyles zlocista poswiate w krzakach...\n";
            std::cout << "Znalazles LEGENDARNY OREZ!\n";

            auto legenda = std::make_unique<RPG::Items::LegendarnaBron>(
                "Ostrze Przeznaczenia", 
                losujLiczbe(40, 60), 
                losujLiczbe(50, 100)
            );
            
            gracz->podniesPrzedmiot(std::move(legenda));
        }
        else if (los <= 25) {
            std::cout << "Znalazles stara bron.\n";
            gracz->podniesPrzedmiot(generujBron(1));
        }
        else if (los <= 40) {
            std::cout << "Znalazles miksture.\n";
            gracz->podniesPrzedmiot(std::make_unique<RPG::Items::Mikstura>(30));
        }
        else {
            walka();
        }
    }

    void Game::walka() {
        std::unique_ptr<RPG::Entities::Przeciwnik> przeciwnik;
        
        int los = losujLiczbe(1, 100);
        
        if (los <= 50) {
            przeciwnik = std::make_unique<RPG::Entities::Wilk>();
        } else if (los <= 75) {
            std::unique_ptr<RPG::Items::Bron> bronBandyty = nullptr;
            if (losujLiczbe(0, 1) == 1) {
                bronBandyty = generujBron(2);
            }
            przeciwnik = std::make_unique<RPG::Entities::Bandyta>(std::move(bronBandyty));
        } else if (los <= 95) {
            przeciwnik = std::make_unique<RPG::Entities::Ork>();
        } else {
            std::cout << "\n!!! CZUJESZ ZAPACH SIARKI... POJAWIA SIE SMOK !!!\n";
            przeciwnik = std::make_unique<RPG::Entities::Smok>();
        }

        std::cout << "Walczysz z: " << przeciwnik->getImie() << "\n";

        while (przeciwnik->czyZyje() && gracz->czyZyje()) {
            std::cout << "\n--- TWOJE HP: " << gracz->getHp() << "/" << gracz->getMaxHp() << " ---\n";
            std::cout << "1. Atakuj\n";
            std::cout << "2. Uciekaj\n";
            std::cout << "3. Uzyj Mikstury (Tracisz ture)\n"; 
            std::cout << "Wybor: ";
            
            int akcja;
            std::cin >> akcja;

            bool turaGraczaWykonana = false;

            if (akcja == 1) {
                gracz->atakuj(*przeciwnik);
                turaGraczaWykonana = true;
            } 
            else if (akcja == 2) {
                if (losujLiczbe(0, 1) == 1) {
                    std::cout << "Udalo ci sie uciec!\n";
                    return;
                } else {
                    std::cout << "Nie udalo sie uciec! Potwor blokuje droge.\n";
                    turaGraczaWykonana = true;
                }
            }
            else if (akcja == 3) {
                if (gracz->uzyjMiksturyWWalce()) {
                    turaGraczaWykonana = true;
                }
            }
            else {
                std::cout << "Nieznana komenda!\n";
            }

            if (turaGraczaWykonana && przeciwnik->czyZyje()) {
                std::cout << "\n>>> Tura Przeciwnika <<<\n";
                przeciwnik->atakuj(*gracz);
            }
        }

        if (gracz->czyZyje()) {
            std::cout << "Zwyciestwo!\n";
            
            gracz->dodajExp(przeciwnik->getExpDrop());
            
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