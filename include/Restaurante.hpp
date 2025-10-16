#pragma once

#include <iostream>
#include <fstream>
#include <queue>
#include <optional>
#include <map>
#include <utility>
#include <string>
#include <vector>
#include "Cozinheiro.hpp"
#include "Mesa.hpp"


class Restaurante{

    private:
    
        std::map<const int, Mesa> mesas;
        std::queue<Cozinheiro> cozinheirosLivre;
        std::queue<int> mesaEmEspera;        
        std::optional<Cozinheiro> getCozinheiro();


    public:
    
        Restaurante(int qntMesas, int qntChefes);

        void receberPedido(int numMesa, std::string pedido);

        void finalizarMesa(int numMesa);
        

};

