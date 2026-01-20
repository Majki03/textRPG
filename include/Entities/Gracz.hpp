#pragma once
#include "Istota.hpp"
#include "../Items/Bron.hpp"
#include "../Items/Przedmiot.hpp"
#include <memory>
#include <vector>

namespace RPG::Entities {

    class Gracz : public Istota {
    private:
        int poziom;
        int exp;
        int expDoNastepnegoPoziomu;
        
        std::unique_ptr<RPG::Items::Bron> bron; 
        
        std::vector<std::unique_ptr<RPG::Items::Przedmiot>> plecak;

    public:
        Gracz(std::string imie, int hp, int sila);

        void atakuj(Istota& cel) override;

        int pobierzObrazenia() const override {
            int dmg = sila;
            if (bron) {
                dmg += bron->getObrazenia();
            }
            return dmg;
        }
        
        void dodajExp(int ilosc);
        void awansuj();

        void podniesBron(std::unique_ptr<RPG::Items::Bron> nowaBron);
        void podniesPrzedmiot(std::unique_ptr<RPG::Items::Przedmiot> przedmiot);
        
        void pokazEkwipunek() const;
        void uzyjPrzedmiotu(int indeks);
        
        bool uzyjMiksturyWWalce();
        
        void zalozBron(std::unique_ptr<RPG::Items::Bron> nowaBron);
    };

}