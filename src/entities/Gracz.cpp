#include "../../include/Entities/Gracz.hpp"
#include "../../include/Items/Mikstura.hpp"
#include "../../include/Items/Bron.hpp"
#include <iostream>

namespace RPG::Entities {

    Gracz::Gracz(std::string imie, int hp, int sila)
        : Istota(imie, hp, sila), poziom(1), exp(0), expDoNastepnegoPoziomu(100) 
    {
    }

    void Gracz::atakuj(Istota& cel) {
        int obrazeniaCalkowite = sila;
        
        if (bron) {
            obrazeniaCalkowite += bron->getObrazenia();
            std::cout << imie << " atakuje uzywajac " << bron->getNazwa() << "!\n";
        } else {
            std::cout << imie << " uderza piesciami!\n";
        }

        cel.otrzymajObrazenia(obrazeniaCalkowite);
    }

    void Gracz::dodajExp(int ilosc) {
        exp += ilosc;
        std::cout << "Zdobywasz " << ilosc << " EXP.\n";

        while (exp >= expDoNastepnegoPoziomu) {
            exp -= expDoNastepnegoPoziomu;
            awansuj();
            expDoNastepnegoPoziomu = static_cast<int>(expDoNastepnegoPoziomu * 1.5);
        }
    }

    void Gracz::awansuj() {
        poziom++;
        sila += 2;
        maxHp += 10;
        hp = maxHp;
        std::cout << ">>> AWANS! " << imie << " osiaga poziom " << poziom << "! <<<\n";
    }

    void Gracz::podniesPrzedmiot(std::unique_ptr<RPG::Items::Przedmiot> przedmiot) {
        std::cout << "Podnosisz: " << przedmiot->getNazwa() << ".\n";
        plecak.push_back(std::move(przedmiot));
    }

    void Gracz::podniesBron(std::unique_ptr<RPG::Items::Bron> nowaBron) {
        podniesPrzedmiot(std::move(nowaBron)); 
    }

    void Gracz::pokazEkwipunek() const {
        std::cout << "\n--- EKWIPUNEK ---\n";
        
        if (bron) {
            std::cout << "[REKA] " << bron->getNazwa() << " (DMG: " << bron->getObrazenia() << ")\n";
        } else {
            std::cout << "[REKA] Pusto\n";
        }

        std::cout << "[PLECAK]:\n";
        if (plecak.empty()) {
            std::cout << "(Pusto)\n";
        } else {
            int i = 0;
            for (const auto& przedmiot : plecak) {
                std::cout << i << ". " << przedmiot->getNazwa();
                
                if (przedmiot->getTyp() == RPG::Items::TypPrzedmiotu::BRON) {
                    auto b = static_cast<RPG::Items::Bron*>(przedmiot.get());
                    std::cout << " (DMG: " << b->getObrazenia() << ")";
                }
                else if (przedmiot->getTyp() == RPG::Items::TypPrzedmiotu::MIKSTURA) {
                     auto m = static_cast<RPG::Items::Mikstura*>(przedmiot.get());
                     std::cout << " (Leczy: " << m->getMoc() << ")";
                }
                
                std::cout << "\n";
                i++;
            }
        }
        std::cout << "-----------------\n";
    }

    void Gracz::uzyjPrzedmiotu(int indeks) {
        if (indeks < 0 || indeks >= plecak.size()) {
            std::cout << "Nieprawidlowy numer przedmiotu.\n";
            return;
        }

        auto& przedmiotPtr = plecak[indeks];

        if (przedmiotPtr->getTyp() == RPG::Items::TypPrzedmiotu::MIKSTURA) {
            auto mikstura = static_cast<RPG::Items::Mikstura*>(przedmiotPtr.get());
            
            lecz(mikstura->getMoc());
            
            std::cout << "Zuzyles " << mikstura->getNazwa() << ".\n";
            plecak.erase(plecak.begin() + indeks);
        }
        else if (przedmiotPtr->getTyp() == RPG::Items::TypPrzedmiotu::BRON) {
            std::unique_ptr<RPG::Items::Przedmiot> item = std::move(plecak[indeks]);
            plecak.erase(plecak.begin() + indeks);

            std::unique_ptr<RPG::Items::Bron> nowaBron(static_cast<RPG::Items::Bron*>(item.release()));

            std::cout << "Wyposazasz: " << nowaBron->getNazwa() << ".\n";

            if (bron) {
                std::cout << "Chowasz " << bron->getNazwa() << " do plecaka.\n";
                plecak.push_back(std::move(bron));
            }

            bron = std::move(nowaBron);
        }
    }
    
    void Gracz::zalozBron(std::unique_ptr<RPG::Items::Bron> nowaBron) {
        if (bron) {
            plecak.push_back(std::move(bron));
        }
        bron = std::move(nowaBron);
        std::cout << imie << " zaklada " << bron->getNazwa() << ".\n";
    }

    bool Gracz::uzyjMiksturyWWalce() {
        std::cout << "\n--- DOSTEPNE MIKSTURY ---\n";
        
        std::vector<int> indeksyMikstur;
        
        for (size_t i = 0; i < plecak.size(); ++i) {
            if (plecak[i]->getTyp() == RPG::Items::TypPrzedmiotu::MIKSTURA) {
                auto m = static_cast<RPG::Items::Mikstura*>(plecak[i].get());
                std::cout << "Indeks [" << i << "]: " << m->getNazwa() << "\n";
                indeksyMikstur.push_back(i);
            }
        }

        if (indeksyMikstur.empty()) {
            std::cout << "Nie posiadasz zadnych mikstur!\n";
            return false;
        }

        std::cout << "Wybierz indeks mikstury (-1 aby wrocic): ";
        int wybor;
        std::cin >> wybor;

        if (wybor == -1) return false;

        bool poprawnyIndeks = false;
        for (int idx : indeksyMikstur) {
            if (wybor == idx) {
                poprawnyIndeks = true;
                break;
            }
        }

        if (poprawnyIndeks) {
            uzyjPrzedmiotu(wybor);
            return true;
        } else {
            std::cout << "To nie jest prawidlowy indeks mikstury!\n";
            return false;
        }
    }

}