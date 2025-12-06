#pragma once

#include <utility>
#include "Cozinheiro.hpp"
#include <unistd.h> 
#include <optional>
#include <string>
class Mesa{


    private:
        const unsigned int ID_MESA;                              

        std::optional<Cozinheiro*> cozinheiro;

        std::queue<std::string> pedidos;


    public:  

        bool estadoDaMesa = false; //Serve para monitorar se a mesa
                                   // já está na fila de espera
        Mesa();

        Mesa(int idMesa);

        std::optional<Cozinheiro*> temCozinheiroLivre();

        void setCozinheiro(Cozinheiro cozinheiro);

        void removerCozinheiro(void);

        void adicionarPedidoEmEspera(const std::string pedido);

        std::string getPedidoEspera();


       
};