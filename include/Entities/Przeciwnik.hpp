#pragma once
#include "Istota.hpp"

namespace RPG::Entities {

    class Przeciwnik : public Istota {
    public:
        Przeciwnik(std::string imie, int hp, int sila)
            : Istota(imie, hp, sila) {}
        
        void atakuj(Istota& cel) override {
            std::cout << imie << " (Potwor) gryzie i drapie!\n";
            cel.otrzymajObrazenia(sila);
        }
    };

    class Goblin : public Przeciwnik {
    public:
        Goblin(std::string imie)
            : Przeciwnik(imie, 50, 5) {}
    };

}