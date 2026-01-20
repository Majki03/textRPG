#pragma once
#include "Bron.hpp"

namespace RPG::Items {

    class LegendarnaBron : public Bron {
    private:
        int bonusHp;

    public:
        LegendarnaBron(std::string nazwa, int obrazenia, int hp)
            : Bron("[LEGENDARY] " + nazwa, obrazenia), bonusHp(hp) {}

        int getBonusHp() const override {
            return bonusHp; 
        }

        std::string getNazwa() const override {
            return "*** " + nazwa + " (HP +" + std::to_string(bonusHp) + ") ***";
        }
    };
}