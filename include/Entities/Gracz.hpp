#pragma once
#include "Istota.hpp"
#include "../Items/Bron.hpp"
#include <memory>

namespace RPG::Entities {
    
    class Gracz : public Istota {
    private:
        int poziom;
        std::unique_ptr<RPG::Items::Bron> bron;

    public:
        Gracz(std::string imie, int hp, int sila);

        void atakuj(Istota& cel) override;

        void zalozBron(std::unique_ptr<RPG::Items::Bron> nowaBron);

        void awansuj();
    };

}