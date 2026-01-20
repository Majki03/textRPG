#pragma once
#include <string>

namespace RPG::Items {

    class Bron {
    public:
        virtual ~Bron() = default;

        virtual std::string getNazwa() const = 0;
        virtual int getObrazenia() const = 0;
    };
}