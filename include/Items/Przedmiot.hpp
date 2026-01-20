#pragma once
#include <string>

namespace RPG::Items {

    enum class TypPrzedmiotu { BRON, MIKSTURA };

    class Przedmiot {
    public:
        virtual ~Przedmiot() = default;
        
        virtual std::string getNazwa() const = 0;
        virtual TypPrzedmiotu getTyp() const = 0;
    };
}