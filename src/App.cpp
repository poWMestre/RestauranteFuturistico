#include "../include/App.hpp"

App::App(){
            
    std::string primeiroDados;
    std::getline(std::cin, primeiroDados);
            
    std::array<std::string, 2> textoQuebrado = devolverStringSeparada(primeiroDados);

    int qntMesa, qntChefe;

    qntChefe = std::stoi(textoQuebrado[0]);
    qntMesa = std::stoi(textoQuebrado[1]);

    Restaurante restaurante(qntMesa, qntChefe);

    while(true){

        std::string pedido;
        std::getline(std::cin , pedido);
                
        if(pedido == "FIM"){break;}

        std::array<std::string, 2> textoQuebrado = devolverStringSeparada(pedido);

        int numMesa = std::stoi(textoQuebrado[0]);

        restaurante.receberPedido(numMesa, textoQuebrado[1]);

    }

};

std::array<std::string, 2> App::devolverStringSeparada(std::string stringInteira){

    std::array<std::string, 2> stringSeparada;
    int indeceEspaco = stringInteira.find(" ");
    stringSeparada[0] = stringInteira.substr(0, indeceEspaco);
    stringSeparada[1] = stringInteira.substr(indeceEspaco + 1);

    return stringSeparada;

}


