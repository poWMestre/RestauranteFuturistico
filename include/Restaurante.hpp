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
    
        std::map<unsigned int, Mesa> mapMesas;
        std::queue<Cozinheiro> listaCozinheiro;
        std::queue<int> ListaMesaEmEspera;

        std::optional<Cozinheiro*> temCozinheiroLivre();

    public:
    
        Restaurante(int qntMesas, int qntChefes);

        void receberPedido(int numMesa, std::string pedido);

        
        

};

