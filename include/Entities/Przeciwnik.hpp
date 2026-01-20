#pragma once
#include "Istota.hpp"
#include "../Items/Bron.hpp"
#include <memory>
#include <iostream>

namespace RPG::Entities {

    class Przeciwnik : public Istota {
    protected:
        int expDrop; // Ile expa daje za zabicie
    public:
        Przeciwnik(std::string imie, int hp, int sila, int exp) 
            : Istota(imie, hp, sila), expDrop(exp) {}

        int getExpDrop() const { return expDrop; }
        
        // --- TO BYŁO BRAKUJĄCE OGNIWO ---
        // Implementacja ataku dla każdego przeciwnika
        void atakuj(Istota& cel) override {
            std::cout << imie << " atakuje!\n";
            cel.otrzymajObrazenia(sila);
        }

        // Metoda wirtualna, aby Bandyta mógł zwrócić łup
        virtual std::unique_ptr<RPG::Items::Bron> upuscBron() {
            return nullptr; // Domyślnie potwory nie mają broni
        }
    };

    // --- RÓŻNE TYPY PRZECIWNIKÓW ---
    
    class Wilk : public Przeciwnik {
    public:
        Wilk() : Przeciwnik("Wilk", 30, 4, 20) {}
    };

    class Ork : public Przeciwnik {
    public:
        Ork() : Przeciwnik("Ork", 80, 12, 100) {}
    };

    class Smok : public Przeciwnik {
    public:
        Smok() : Przeciwnik("Smok Wawelski", 300, 40, 2000) {}
    };

    class Bandyta : public Przeciwnik {
    private:
        std::unique_ptr<RPG::Items::Bron> bron;
    public:
        Bandyta(std::unique_ptr<RPG::Items::Bron> bronBandyty) 
            : Przeciwnik("Bandyta", 60, 8, 80) {
            
            if (bronBandyty) {
                this->sila += bronBandyty->getObrazenia();
                this->imie += " z bronia (" + bronBandyty->getNazwa() + ")";
                this->bron = std::move(bronBandyty);
            }
        }

        std::unique_ptr<RPG::Items::Bron> upuscBron() override {
            if (bron) {
                std::cout << "Bandyta upuszcza swoja bron!\n";
                return std::move(bron);
            }
            return nullptr;
        }
    };
}