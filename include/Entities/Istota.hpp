#pragma once
#include <string>
#include <iostream>
#include "../Core/GameException.hpp"

namespace RPG::Entities {

    class Istota {
    protected:
        std::string imie;
        int hp;
        int maxHp;
        int sila;

    public:
        Istota(std::string imie, int hp, int sila);

        virtual ~Istota() = default;

        std::string getImie() const;
        int getHp() const;
        int getMaxHp() const;
        int czyZyje() const;

        void otrzymajObrazenia(int punkty);
        void lecz(int punkty);

        virtual void atakuj(Istota& cel) = 0;
    };
    
}