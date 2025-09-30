#include "../include/Mesa.h"
 
Mesa::Mesa() : ID_MESA(0){};

Mesa::Mesa(int idMesa) : ID_MESA(idMesa){

    pipe(fdComunicacao);

};

std::optional<Cozinheiro> Mesa::MesaTemCozinheiro(){
    
    if(this->cozinheiro.has_value()){
        return this->cozinheiro;
    }
    
    return std::nullopt;
}

void Mesa::setCozinheiro(Cozinheiro cozinheiro){
    this->cozinheiro.emplace(cozinheiro);
}




