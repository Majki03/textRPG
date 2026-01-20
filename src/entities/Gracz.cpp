#include "../../include/Entities/Gracz.hpp"
#include "../../include/Items/Mikstura.hpp"
#include "../../include/Items/Bron.hpp"
#include "../../include/Items/LegendarnaBron.hpp"
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
            expDoNastepnegoPoziomu = static_cast<int>(expDoNastepnegoPoziomu * 1.2);
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
                    std::cout << " (DMG: " << b->getObrazenia();
                    
                    if (b->getBonusHp() > 0) {
                        std::cout << ", HP: +" << b->getBonusHp();
                    }
                    std::cout << ")";
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
                int staryBonus = bron->getBonusHp();
                if (staryBonus > 0) {
                    maxHp -= staryBonus;
                    if (hp > maxHp) hp = maxHp;
                    std::cout << "Tracisz bonus broni: -" << staryBonus << " Max HP.\n";
                }

                std::cout << "Chowasz " << bron->getNazwa() << " do plecaka.\n";
                plecak.push_back(std::move(bron));
            }

            int nowyBonus = nowaBron->getBonusHp();
            if (nowyBonus > 0) {
                maxHp += nowyBonus;
                hp += nowyBonus;
                std::cout << "Moc legendy przeplywa przez ciebie! +" << nowyBonus << " Max HP!\n";
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

    void Gracz::zapisz(std::ofstream& plik) const {
        plik << imie << std::endl;
        plik << hp << std::endl;
        plik << maxHp << std::endl;
        plik << sila << std::endl;
        plik << poziom << std::endl;
        plik << exp << std::endl;
        plik << expDoNastepnegoPoziomu << std::endl;

        plik << plecak.size() << std::endl;
        for (const auto& item : plecak) {
            int typ = (item->getTyp() == RPG::Items::TypPrzedmiotu::BRON) ? 0 : 1;
            plik << typ << std::endl;
            
            plik << item->getNazwa() << std::endl;

            if (typ == 0) {
                auto b = static_cast<RPG::Items::Bron*>(item.get());
                plik << b->getObrazenia() << std::endl;
                plik << b->getBonusHp() << std::endl;
            } else {
                auto m = static_cast<RPG::Items::Mikstura*>(item.get());
                plik << m->getMoc() << std::endl;
            }
        }

        if (bron) {
            plik << "1" << std::endl;
            plik << bron->getNazwa() << std::endl;
            plik << bron->getObrazenia() << std::endl;
            plik << bron->getBonusHp() << std::endl;
        } else {
            plik << "0" << std::endl;
        }
    }

    void Gracz::wczytaj(std::ifstream& plik) {
        plecak.clear();
        bron = nullptr;

        auto safeGetLine = [&](std::string& s) {
            char temp;
            plik.get(temp);
            std::getline(plik, s); 
        };

        plik >> imie; 
        plik >> hp;
        plik >> maxHp;
        plik >> sila;
        plik >> poziom;
        plik >> exp;
        plik >> expDoNastepnegoPoziomu;

        int rozmiarPlecaka;
        plik >> rozmiarPlecaka;

        for(int i=0; i<rozmiarPlecaka; ++i) {
            int typ;
            plik >> typ;
            
            std::string nazwa;
            safeGetLine(nazwa);
            
            if (typ == 0) {
                int dmg, bonusHp;
                plik >> dmg >> bonusHp;
                
                if (bonusHp > 0) {
                    plecak.push_back(std::make_unique<RPG::Items::LegendarnaBron>(nazwa, dmg, bonusHp));
                } else {
                    plecak.push_back(std::make_unique<RPG::Items::Bron>(nazwa, dmg));
                }
            } else {
                int moc;
                plik >> moc;
                plecak.push_back(std::make_unique<RPG::Items::Mikstura>(moc));
            }
        }

        int maBron;
        plik >> maBron;
        if (maBron == 1) {
            std::string nazwa;
            safeGetLine(nazwa);
            int dmg, bonusHp;
            plik >> dmg >> bonusHp;
            
            if (bonusHp > 0) {
                bron = std::make_unique<RPG::Items::LegendarnaBron>(nazwa, dmg, bonusHp);
            } else {
                bron = std::make_unique<RPG::Items::Bron>(nazwa, dmg);
            }
        }
        
        std::cout << ">>> Gra wczytana pomyslnie! <<<\n";
    }

}