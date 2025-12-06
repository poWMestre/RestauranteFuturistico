#include "../include/Mesa.hpp"
 
Mesa::Mesa() : ID_MESA(0){};

Mesa::Mesa(int idMesa) : ID_MESA(idMesa){};

std::optional<Cozinheiro*> Mesa::temCozinheiroLivre(){
    
    if(this->cozinheiro.has_value()){
        return this->cozinheiro;
    }
    
    return std::nullopt;
}

void Mesa::setCozinheiro(Cozinheiro cozinheiro){
    this->cozinheiro = &cozinheiro;
}

void Mesa::removerCozinheiro(){
    this->cozinheiro.reset();
}

void Mesa::adicionarPedidoEmEspera(const std::string pedido){
    this->pedidos.push(pedido);
}

std::string Mesa::getPedidoEspera(){

    if(this->pedidos.empty()){
        return "";
    }

    std::string p = this->pedidos.front(); 
    this->pedidos.pop();
    return p;
}


