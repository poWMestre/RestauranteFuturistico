#pragma once

#include <utility>
#include "Cozinheiro.hpp"
#include <unistd.h> 
#include <optional>
#include <string>
class Mesa{


    private:
        const unsigned int ID_MESA;
        int estadoDaMesa; // 0 = mesa parada 
                              // 1 = Mesa na fila
                               

        std::optional<Cozinheiro> cozinheiro;

        std::queue<std::string> pedidos;
        
    
    public:  

        Mesa();

        Mesa(int idMesa);

        std::optional<Cozinheiro> MesaTemCozinheiro();

        void setCozinheiro(Cozinheiro cozinheiro);

        void removerCozinheiro(void);

        void adicionarPedidoEmEspera(const std::string pedido);

        void getPedidoEspera();

        void trocarEstadoDaMesa(int* num);

        void getEstadoDaMesa();

};