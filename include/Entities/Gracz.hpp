#pragma once
#include "Istota.hpp"
#include "../Items/Bron.hpp"
#include "../Items/Przedmiot.hpp" // Ważne!
#include <memory>
#include <vector>

namespace RPG::Entities {

    class Gracz : public Istota {
    private:
        int poziom;
        int exp;
        int expDoNastepnegoPoziomu;
        
        // Broń w ręce
        std::unique_ptr<RPG::Items::Bron> bron; 
        
        // Plecak musi przechowywać ogólny PRZEDMIOT, nie tylko BRON
        std::vector<std::unique_ptr<RPG::Items::Przedmiot>> plecak;

    public:
        Gracz(std::string imie, int hp, int sila);

        void atakuj(Istota& cel) override;
        
        void dodajExp(int ilosc);
        void awansuj();

        // Metody ekwipunku
        void podniesBron(std::unique_ptr<RPG::Items::Bron> nowaBron); // Stara metoda (dla kompatybilności)
        void podniesPrzedmiot(std::unique_ptr<RPG::Items::Przedmiot> przedmiot);
        
        void pokazEkwipunek() const;
        void uzyjPrzedmiotu(int indeks); // Zastępuje zmienBron
        
        // Metoda pomocnicza dla Game.cpp, jeśli potrzebna
        void zalozBron(std::unique_ptr<RPG::Items::Bron> nowaBron);
    };

}