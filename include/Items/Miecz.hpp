#pragma once
#include "Bron.hpp"

namespace RPG::Items {

    class Miecz : public Bron {
    private:
        std::string nazwa;
        int obrazenia;

    public:
        Miecz(std::string nazwa, int obrazenia)
            : nazwa(nazwa), obrazenia(obrazenia) {}
        
        std::string getNazwa() const override { return nazwa; }
        int getObrazenia() const override { return obrazenia; }
    };

}