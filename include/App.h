#pragma once

#include <iostream>
#include <string>
#include <array>
#include "Restaurante.h"


class App{

    public:

        App();

        std::array<std::string, 2> devolverStringSeparada(std::string stringTexto);

};