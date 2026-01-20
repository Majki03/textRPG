#pragma once
#include <memory>
#include "../Entities/Gracz.hpp"

namespace RPG::Core {

    class Game {
    private:
        std::unique_ptr<RPG::Entities::Gracz> gracz;

        bool isRunning;

        void menuGlowne();
        void eksploruj();
        void walka();
        void walkaZBossem();
        void zapiszGre();
        void wczytajGre();

        int losujLiczbe(int min, int max);

    public:
        Game();
        void run();
    };
    
}