#include "../../include/Entities/Gracz.hpp"
#include <iostream>

namespace RPG::Entities {

    Gracz::Gracz(std::string imie, int hp, int sila)
        : Istota(imie, hp, sila), poziom(1)
        {
        }
    
        void Gracz::atakuj(Istota& cel) {
            int obrazeniaCalkowite = sila;

            if (bron) {
                obrazeniaCalkowite += bron->getObrazenia();
                std::cout << imie << " atakuje uzywajac " << bron->getNazwa() << "!\n";
            } else {
                std::cout << imie << " atakuje piescami!\n";
            }

            cel.otrzymajObrazenia(obrazeniaCalkowite);
        }

        void Gracz::podniesBron(std::unique_ptr<RPG::Items::Bron> nowaBron) {
            std::cout << "Podnoszisz przedmiot: " << nowaBron->getNazwa() << ".\n";
            
            plecak.push_back(std::move(nowaBron));
        }

        void Gracz::pokazEkwipunek() const {
            std::cout << "\n--- Ekwipunek ---\n";

            if (bron) {
                std::cout << "W rece: " << bron->getNazwa() << " (DMG: " << bron->getObrazenia() << ")\n";
            } else {
                std::cout << "W rece [PUSTO]\n";
            }

            std::cout << "W plecku:\n";
            if (plecak.empty()) {
                std::cout << "(Pusto)\n";
            } else {
                int i = 0;
                for (const auto& przedmiot : plecak) {
                    std::cout << i << ". " << przedmiot->getNazwa()
                                << " (DMG: " << przedmiot->getObrazenia() << ")\n";
                    i++;
                }
            }
            std::cout << "-----------------\n";
        }

        void Gracz::zmienBron(int indeks) {
            if (indeks < 0 || indeks >= plecak.size()) {
                std::cout << "Nie ma takiego przedmiotu!\n";
                return;
            }

            std::cout << "Zmieniasz bron na: " << plecak[indeks]->getNazwa() << ".\n";

            if (bron == nullptr) {
                bron = std::move(plecak[indeks]);
                plecak.erase(plecak.begin() + indeks);
            } else {
                std::swap(bron, plecak[indeks]);
            }
        }

        void Gracz::awansuj() {
            poziom++;
            sila += 2;
            maxHp += 10;
            hp = maxHp;
            std::cout << imie << " awansuje na poziom " << poziom << "!\n";
        }
        
}