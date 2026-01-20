#pragma once
#include "Istota.hpp"
#include "../Items/Bron.hpp"
#include <memory>
#include <vector>

namespace RPG::Entities {
    
    class Gracz : public Istota {
    private:
        int poziom;
        std::unique_ptr<RPG::Items::Bron> bron;

        std::vector<std::unique_ptr<RPG::Items::Bron>> plecak;

    public:
        Gracz(std::string imie, int hp, int sila);

        void atakuj(Istota& cel) override;

        void podniesBron(std::unique_ptr<RPG::Items::Bron> nowaBron);

        void pokazEkwipunek() const;
        void zmienBron(int indeks);

        void awansuj();
    };

}