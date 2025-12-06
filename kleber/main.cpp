#include <iostream>
#include <thread>
#include <vector>

#include "chef.hpp"

int main() {
    //signed(SIGCHLD, SIG_IGN); // evita filhos zumbis (pesquisem sobre...)

    // cria um std::vector (estrutura semelhante ao ArrayList) e adiciona 3 objetos Chef com IDs de 1 a 3
    std::vector<Chef> chefs;
    chefs.emplace_back(1);
    chefs.emplace_back(2);
    chefs.emplace_back(3);

    while (true) {
        int c;
        std::string pedido;

        // lê o inteiro e consome o \n ao final da linha
        std::cin >> c;
        std::cin.ignore();
        // se o inteiro for 0, sai deste laço e encerra o programa
        if (c == 0) break;

        // lê a linha inteira e coloca na string pedido
        std::getline(std::cin, pedido);

        if (pedido.empty() || pedido == "fim") {
            chefs.at(c - 1).encerrarAtendimento();
        } else {
            chefs.at(c - 1).prepararPedido(pedido);
        }
    }

    return EXIT_SUCCESS;
}
