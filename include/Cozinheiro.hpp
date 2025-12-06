#pragma once
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <signal.h>

class Atendimento{

    private:

        int fd[2]{-1, -1};
        pid_t pid;
        unsigned int idCozinheiro;
        unsigned int idMesa;


    public:

        Atendimento(unsigned int idCozinheiro, unsigned int idMesa);

        ~Atendimento();

        void prepararPedidoEscrita(const std::string &pedido);

        void iniciar();


};

class Cozinheiro{
    
    private:

        const unsigned int ID_COZINHEIRO;
        Atendimento *atendimento;
    
    public:


        Cozinheiro(unsigned int idCozinheiro);

        void prepararPedido(const std::string &pedido, int numMesa);

        void iniciarAtendimento(unsigned int mesa);

        void finalizarAtendimento();

};