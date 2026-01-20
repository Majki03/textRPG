#pragma once
#include "Przedmiot.hpp"

namespace RPG::Items {

    class Bron : public Przedmiot {
    protected:
        std::string nazwa;
        int obrazenia;
    
    public:
        Bron(std::string nazwa, int obrazenia)
            : nazwa(nazwa), obrazenia(obrazenia) {}

        std::string getNazwa() const override { return nazwa; }
        TypPrzedmiotu getTyp() const override { return TypPrzedmiotu::BRON; }

        int getObrazenia() const { return obrazenia; }

        virtual int getBonusHp() const { return 0; }
    };
}