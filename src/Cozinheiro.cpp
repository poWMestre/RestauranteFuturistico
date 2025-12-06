#include "../include/Cozinheiro.hpp"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>

Atendimento::Atendimento(unsigned int idCozinheiro, unsigned int idMesa){

    this->idCozinheiro = idCozinheiro;
    this->idMesa = idMesa;

    pipe(fd); 

    this->pid = fork();

    if(pid < 0){
        return;
    }else if(pid == 0){

        close(fd[1]);  
        iniciar();
        _exit(0);  

    }else{

        close(fd[0]);  
    }

}

Atendimento::~Atendimento(){
    close(fd[1]);
    
    pid_t pid = getpid();

    if(pid == this->pid){
        kill(pid, SIGKILL);
    }
    
}

void Atendimento::prepararPedidoEscrita(const std::string &pedido){

    
    std::string nomeArquivo = "../Cozinheiro_" + std::to_string(this->idCozinheiro) + ".txt";

    std::ofstream arquivo(nomeArquivo);

    if(!arquivo.is_open()){
        return;
    }

    arquivo << pedido << "\n";

    arquivo.close();

    
    
}

void Atendimento::iniciar(){

    close(fd[1]); 

    char buffer[512];

    
    while(true){
        const ssize_t lerPedido = read(fd[0], buffer, sizeof(buffer) - 1);

        if(lerPedido <= 0) break;

        buffer[lerPedido] = '\0';
        
        std::string pedido(buffer);

    }

    close(fd[0]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Cozinheiro::Cozinheiro(unsigned int idCozinheiro) : ID_COZINHEIRO(idCozinheiro), 
            atendimento(nullptr){}

void Cozinheiro::iniciarAtendimento(unsigned int Mesa){

    this->atendimento = new Atendimento(this->ID_COZINHEIRO, Mesa);

}

void Cozinheiro::prepararPedido(const std::string &pedido, int numMesa){

    if(this->atendimento == nullptr){
        iniciarAtendimento(0);
    }

    this->atendimento->prepararPedidoEscrita(pedido);

    if(pedido == "fim"){
        finalizarAtendimento();

        this->atendimento->~Atendimento();
    }

}

void Cozinheiro::finalizarAtendimento(){

    delete atendimento;
    atendimento = nullptr;
}
