#pragma once

#include <utility>
#include "Cozinheiro.h"
#include <unistd.h> 
#include <optional>
#include <string>
class Mesa{


    private:
        const unsigned int ID_MESA;

        std::optional<Cozinheiro> cozinheiro;

        int fdComunicacao[2];
    
    public:  

        Mesa();

        Mesa(int idMesa);

        std::optional<Cozinheiro> MesaTemCozinheiro();

        void setCozinheiro(Cozinheiro cozinheiro);

        int getFD();
};