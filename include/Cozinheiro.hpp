#pragma once
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
class Cozinheiro{
    
    private:

        const unsigned int ID_COZINHEIRO;

        int fdComunicacao[2];
    
    public:

        Cozinheiro();

        Cozinheiro(int idCozinheiro);

        void prepararPedido(std::string pedido);

        int* getFD();


};