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

        void Gracz::zalozBron(std::unique_ptr<RPG::Items::Bron> nowaBron) {
            bron = std::move(nowaBron);

            std::cout << imie << " wyposaza " << bron->getNazwa() << ".\n";
        }

        void Gracz::awansuj() {
            poziom++;
            sila += 2;
            maxHp += 10;
            hp = maxHp;
            std::cout << imie << " awansuje na poziom " << poziom << "!\n";
        }
        
}