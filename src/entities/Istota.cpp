#include "../../include/Entities/Istota.hpp"

namespace RPG::Entities {

    Istota::Istota(std::string imie, int hp, int sila)
        : imie(imie), hp(hp), maxHp(hp), sila(sila) 
    {
        if (hp <= 0) {
            throw RPG::Core::GameException("Nie mozna stworzyc istoty z HP mniejszym lub rownym 0!");
        }
        if (sila < 0) {
            throw RPG::Core::GameException("Nie mozna stworzyc istoty z sila mniejsza od 0!");
        }
        if (imie.empty()) {
            throw RPG::Core::GameException("Nie mozna stworzyc istoty bez imienia!");
        }
    }

    std::string Istota::getImie() const {
        return imie;
    }

    int Istota::getHp() const {
        return hp;
    }

    int Istota::getMaxHp() const {
        return maxHp;
    }

    int Istota::czyZyje() const {
        return hp > 0;
    }

    void Istota::otrzymajObrazenia(int punkty) {
        if (punkty < 0) return;

        hp -= punkty;
        if (hp < 0) hp = 0;

        std::cout << imie << " otrzymuje " << punkty << " obtrazen! (HP: " << hp << "/" << maxHp << ")\n";
    }

    void Istota::lecz(int punkty) {
        if (!czyZyje()) {
            throw RPG::Core::GameException("Nie mozna wyleczyc martwej istoty!");
        }

        hp += punkty;
        if (hp > maxHp) hp = maxHp;

        std::cout << imie << " zostaje wyleczony o " << punkty << " punktow! (HP: " << hp << "/" << maxHp << ")\n";
    }

}