#include "../include/Cozinheiro.h"

Cozinheiro::Cozinheiro() : ID_COZINHEIRO(0){};

Cozinheiro::Cozinheiro(int idCozinheiro) : ID_COZINHEIRO(idCozinheiro){};

void Cozinheiro::prepararPedido(int numMesa, std::string pedido){
    
    std::string nomeFile = "Cozinheiro_" + std::to_string(this->ID_COZINHEIRO) + ".txt";

    std::fstream openFile(nomeFile);

    if(openFile.is_open()){

        openFile << "Welder";

        openFile.close();
    }
}