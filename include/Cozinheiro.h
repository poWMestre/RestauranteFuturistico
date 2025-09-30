#pragma once
#include <iostream>
#include <fstream>
#include <string>
class Cozinheiro{
    
    private:

        const unsigned int ID_COZINHEIRO;
    
    public:

        Cozinheiro();

        Cozinheiro(int idCozinheiro);

        void prepararPedido(int numMesa, std::string pedido);


};