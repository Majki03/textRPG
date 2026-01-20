#pragma once
#include "Przedmiot.hpp"

namespace RPG::Items {

    class Mikstura : public Przedmiot {
    private:
        int mocLeczenia;
        
    public:
        Mikstura(int moc) : mocLeczenia(moc) {}

        std::string getNazwa() const override { 
            return "Mikstura Leczenia (" + std::to_string(mocLeczenia) + " HP)"; 
        }
        
        TypPrzedmiotu getTyp() const override { return TypPrzedmiotu::MIKSTURA; }
        
        int getMoc() const { return mocLeczenia; }
    };
}