#include "../include/Restaurante.hpp"

Restaurante::Restaurante(int qntMesas, int qntChefes){
            
    int i, j;

    for(i = 1; i <= qntMesas; i++){

        this->mapMesas.insert({i, Mesa(i)});

    }

    for(j = 1; j <= qntChefes; j++){

        this->listaCozinheiro.push(Cozinheiro(j));

        std::ofstream outputFile("Cozinheiro_" + std::to_string(j) + ".txt");

    }

};


void Restaurante::receberPedido(int numMesa, const std::string pedido){
    
    Mesa* mesaDaVez = &this->mapMesas[numMesa];
    std::optional<Cozinheiro*> cozinheiro;

    std::optional<Cozinheiro*> cozinheiroDaMesa = mesaDaVez->temCozinheiroLivre();


    //Verifica se não tem cozinheiro já atendendo a mesa
    if(!cozinheiroDaMesa.has_value()){
        
        //verifica se tem mesa em fila de espera
        if(!this-> ListaMesaEmEspera.empty()){

            if(!mesaDaVez->estadoDaMesa){

                mesaDaVez->adicionarPedidoEmEspera(pedido);

                this->ListaMesaEmEspera.push(numMesa);

                mesaDaVez->estadoDaMesa = true;

            }
               
            numMesa = this->ListaMesaEmEspera.front();

            this->ListaMesaEmEspera.pop();

            mesaDaVez = &this->mapMesas[numMesa];
        }

        cozinheiro = temCozinheiroLivre();

        if(!cozinheiro.has_value()){
            
            mesaDaVez->adicionarPedidoEmEspera(pedido);
            
            return;    

        }else{

            mesaDaVez->setCozinheiro(*cozinheiro.value());

        }

    }

    mesaDaVez->adicionarPedidoEmEspera(pedido);

    std::string pedidoEmEspera;

    while((pedidoEmEspera = mesaDaVez->getPedidoEspera()) != ""){

        cozinheiro.value()->prepararPedido(pedidoEmEspera, numMesa);

    }

}



std::optional<Cozinheiro*> Restaurante::temCozinheiroLivre(){

    if(listaCozinheiro.empty()){
        return std::nullopt;
    }

    Cozinheiro cozinheiro = this->listaCozinheiro.front();
    this->listaCozinheiro.pop();

    return &cozinheiro;

}