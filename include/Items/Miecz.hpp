#pragma once
#include "Bron.hpp"

namespace RPG::Items {

    class Miecz : public Bron {
    public:
        // Wywołujemy konstruktor klasy bazowej Bron
        Miecz(std::string nazwa, int obrazenia) 
            : Bron(nazwa, obrazenia) {}

        // Nie musimy nadpisywać getNazwa() ani getObrazenia(),
        // ponieważ klasa Bron już to robi za nas!
    };

}