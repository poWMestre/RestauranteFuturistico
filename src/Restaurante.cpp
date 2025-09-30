#include "../include/Restaurante.h"

std::optional<Cozinheiro> Restaurante::getCozinheiro(){
            
    if(this->cozinheirosLivre.empty()){
        return std::nullopt;
    }

    Cozinheiro &tmpCozinheiro = this->cozinheirosLivre.front();
    this->cozinheirosLivre.pop();
    return tmpCozinheiro;
} 

Restaurante::Restaurante(int qntMesas, int qntChefes){
            
    int i, j;

    for(i = 1; i <= qntMesas; i++){

        this->mesas.emplace(std::piecewise_construct, //Estudar isso.
                            std::forward_as_tuple(i),
                            std::forward_as_tuple(i));

    }

    for(j = 1; j <= qntChefes; j++){

        this->cozinheirosLivre.emplace(Cozinheiro(j));

        std::ofstream outputFile("Cozinheiro_" + std::to_string(j) + ".txt");

    }

};

void Restaurante::receberPedido(int numMesa, std::string pedido){
    
    Mesa &mesaDaVez = this->mesas[numMesa];

    if(!mesaDaVez.MesaTemCozinheiro().has_value()){
        
        std::optional<Cozinheiro> cozinheiro = getCozinheiro();
       
        if(!cozinheiro.has_value()){
            //Colocar na fila de espera
            return;
        }

        mesaDaVez.setCozinheiro(cozinheiro.value());
    } 

    Cozinheiro cozinheiro = mesaDaVez.MesaTemCozinheiro().value();

    cozinheiro.prepararPedido(numMesa, pedido);

    


}

   