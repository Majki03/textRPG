#include "../../include/Core/Game.hpp"
#include "../../include/Entities/Przeciwnik.hpp"
#include "../../include/Items/Miecz.hpp"
#include "../../include/Items/Mikstura.hpp"
#include "../../include/Items/Przedmiot.hpp"
#include "../../include/Items/LegendarnaBron.hpp"
#include <iostream>
#include <random>
#include <ctime>
#include <fstream>

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
        std::cout << "4. WALKA Z BOSSEM (Koniec Gry)\n";
        std::cout << "5. Zapisz gre\n";
        std::cout << "6. Wczytaj gre\n";
        std::cout << "7. Wyjdz z gry\n";
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
            case 4: walkaZBossem(); break;
            case 5: zapiszGre(); break;
            case 6: wczytajGre(); break;
            case 7:
                std::cout << "Zamykanie gry...\n";
                isRunning = false; 
                break;
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

        if (los == 1) { 
            std::cout << "\n!!! NIEWIARYGODNE !!! Znalazles LEGENDARNY OREZ!\n";
            auto legenda = std::make_unique<RPG::Items::LegendarnaBron>(
                "Ostrze Przeznaczenia", losujLiczbe(50, 70), losujLiczbe(100, 200)
            );
            gracz->podniesPrzedmiot(std::move(legenda));
        }
        else if (los <= 10) {
            std::cout << "Znalazles stara bron.\n";
            gracz->podniesPrzedmiot(generujBron(1));
        }
        else if (los <= 20) {
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
        if (los <= 50) przeciwnik = std::make_unique<RPG::Entities::Wilk>();
        else if (los <= 75) {
             std::unique_ptr<RPG::Items::Bron> bronBandyty = nullptr;
             if (losujLiczbe(0, 1) == 1) bronBandyty = generujBron(2);
             przeciwnik = std::make_unique<RPG::Entities::Bandyta>(std::move(bronBandyty));
        }
        else if (los <= 95) przeciwnik = std::make_unique<RPG::Entities::Ork>();
        else przeciwnik = std::make_unique<RPG::Entities::Smok>();

        std::cout << "Walczysz z: " << przeciwnik->getImie() << "\n";

        while (przeciwnik->czyZyje() && gracz->czyZyje()) {
            std::cout << "\n--- TWOJE HP: " << gracz->getHp() << " | WROG: " << przeciwnik->getHp() << " ---\n";
            std::cout << "1. Atakuj  2. Uciekaj  3. Mikstura\nWybor: ";
            
            int akcja;
            std::cin >> akcja;
            bool turaGraczaWykonana = false;

            if (akcja == 1) {
                gracz->atakuj(*przeciwnik);
                turaGraczaWykonana = true;
            } 
            else if (akcja == 2) {
                if (losujLiczbe(0, 1) == 1) { std::cout << "Uciekles!\n"; return; }
                else { std::cout << "Ucieczka nieudana!\n"; turaGraczaWykonana = true; }
            }
            else if (akcja == 3) {
                if (gracz->uzyjMiksturyWWalce()) turaGraczaWykonana = true;
            }

            if (turaGraczaWykonana && przeciwnik->czyZyje()) {
                std::cout << "\n>>> Tura Przeciwnika <<<\n";
                
                int szansaNaObrone = losujLiczbe(1, 100);
                
                if (szansaNaObrone <= 10) {
                    std::cout << "!!! WSPANIALY UNIK! Nie otrzymujesz obrazen! !!!\n";
                }
                else if (szansaNaObrone <= 25) {
                    int dmgWroga = przeciwnik->pobierzObrazenia();
                    int zredukowaneDmg = dmgWroga / 2;
                    
                    std::cout << "!!! ZABLOKOWALES ATAK! (Obrazenia: " << dmgWroga << " -> " << zredukowaneDmg << ") !!!\n";
                    gracz->otrzymajObrazenia(zredukowaneDmg);

                    int naszDmg = gracz->pobierzObrazenia() / 2;
                    std::cout << "!!! SZYBKI KONTRATAK! Zadajesz " << naszDmg << " obrazen! !!!\n";
                    przeciwnik->otrzymajObrazenia(naszDmg);
                }
                else {
                    przeciwnik->atakuj(*gracz);
                }
            }
        }

        if (gracz->czyZyje()) {
            std::cout << "Zwyciestwo!\n";
            gracz->dodajExp(przeciwnik->getExpDrop());
            auto lup = przeciwnik->upuscBron();
            if (lup) gracz->podniesPrzedmiot(std::move(lup));
        }
    }

    int Game::losujLiczbe(int min, int max) {
        static std::mt19937 generator(std::time(nullptr));
        std::uniform_int_distribution<int> dystrybucja(min, max);
        return dystrybucja(generator);
    }

    void Game::walkaZBossem() {
        std::cout << "\n\n=========================================\n";
        std::cout << "WCHODZISZ DO KOMNATY MROCZNEGO PANA...\n";
        std::cout << "=========================================\n";
        
        if (gracz->getMaxHp() < 200) {
            std::cout << "Twoj bohater trzesie sie ze strachu... Jestes za slaby!\n";
        }

        auto boss = std::make_unique<RPG::Entities::Przeciwnik>("MROCZNY PAN", 1000, 60, 10000);
        
        while (boss->czyZyje() && gracz->czyZyje()) {
            std::cout << "\n[BOSS] " << boss->getImie() << " HP: " << boss->getHp() << "\n";
            std::cout << "[TY]   " << gracz->getImie() << " HP: " << gracz->getHp() << "/" << gracz->getMaxHp() << "\n";
            
            std::cout << "1. Atakuj  2. Mikstura (Ucieczka niemozliwa!)\nWybor: ";
            int akcja;
            std::cin >> akcja;
            bool turaGracza = false;

            if (akcja == 1) {
                gracz->atakuj(*boss);
                turaGracza = true;
            } else if (akcja == 2) {
                if (gracz->uzyjMiksturyWWalce()) turaGracza = true;
            }

            if (turaGracza && boss->czyZyje()) {
                std::cout << "\n>>> Mroczny Pan przygotowuje potezny cios... <<<\n";
                
                int szansa = losujLiczbe(1, 100);
                if (szansa <= 10) {
                    std::cout << "Cudem uniknales smiertelnego ciosu!\n";
                } else if (szansa <= 25) {
                    int dmg = boss->pobierzObrazenia() / 2;
                    std::cout << "Zablokowales czesc uderzenia! Otrzymujesz " << dmg << " obrazen.\n";
                    gracz->otrzymajObrazenia(dmg);
                    
                    int kontra = gracz->pobierzObrazenia() / 2;
                    std::cout << "Kontratakujesz za " << kontra << " pkt!\n";
                    boss->otrzymajObrazenia(kontra);
                } else {
                    boss->atakuj(*gracz);
                }
            }
        }

        if (gracz->czyZyje()) {
            std::cout << "\n\n*****************************************\n";
            std::cout << "       GRATULACJE! UKONCZYLES GRE!       \n";
            std::cout << "   Mroczny Pan upadl, a swiat jest wolny.  \n";
            std::cout << "*****************************************\n\n";
            isRunning = false;
        } else {
            std::cout << "\nZostales zmiazdzony przez Mrocznego Pana...\nGame Over.\n";
            isRunning = false;
        }
    }

    void Game::zapiszGre() {
        std::ofstream plik("save_rpg.txt");
        if (plik.is_open()) {
            gracz->zapisz(plik);
            plik.close();
            std::cout << "\n[SYSTEM] Gra zostala zapisana do pliku 'save_rpg.txt'.\n";
        } else {
            std::cout << "\n[BLAD] Nie udalo sie otworzyc pliku do zapisu!\n";
        }
    }

    void Game::wczytajGre() {
        std::ifstream plik("save_rpg.txt");
        if (plik.is_open()) {
            // Sprawdź czy plik nie jest pusty (prostym sposobem)
            if (plik.peek() == std::ifstream::traits_type::eof()) {
                std::cout << "\n[BLAD] Plik zapisu jest pusty lub uszkodzony.\n";
                return;
            }

            try {
                gracz->wczytaj(plik);
            } catch (const std::exception& e) {
                std::cout << "\n[BLAD] Plik zapisu jest uszkodzony! " << e.what() << "\n";
            }
            
            plik.close();
        } else {
            std::cout << "\n[INFO] Nie znaleziono pliku zapisu.\n";
        }
    }
    
}