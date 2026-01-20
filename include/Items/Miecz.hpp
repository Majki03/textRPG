#pragma once
#include "Bron.hpp"

namespace RPG::Items {

    class Miecz : public Bron {
    public:
        Miecz(std::string nazwa, int obrazenia) 
            : Bron(nazwa, obrazenia) {}
    };

}