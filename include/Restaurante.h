#pragma once

#include <iostream>
#include <fstream>
#include <queue>
#include <optional>
#include <map>
#include <utility>
#include <string>
#include <vector>
#include "Cozinheiro.h"
#include "Mesa.h"


class Restaurante{

    private:

        std::vector<std::vector<int>> fd;
    
        std::map<const int, Mesa> mesas;
        std::queue<Cozinheiro> cozinheirosLivre;
        std::queue<std::pair<unsigned int, std::queue<std::string>>> mesasEmEspera;
        
        std::optional<Cozinheiro> getCozinheiro();


    public:
    
        Restaurante(int qntMesas, int qntChefes);

        void receberPedido(int numMesa, std::string pedido);
        

};

