#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <string>
#include "lista.hpp" 

class Cliente {
public:
    std::string nome;
    ListaEncadeada<int> pacotes; 

    Cliente(const std::string& n = "") : nome(n) {}

};

#endif 