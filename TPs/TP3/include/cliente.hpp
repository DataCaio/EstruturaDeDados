#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <string>
#include "lista.hpp" // Your custom linked list

class Cliente {
public:
    std::string nome;
    ListaEncadeada<int> pacotes; // Store UNIQUE IDs of packages associated with this client

    Cliente(const std::string& n = "") : nome(n) {}

    // Comparison operators for AVLTree key
    bool operator<(const Cliente& other) const { return nome < other.nome; }
    bool operator>(const Cliente& other) const { return nome > other.nome; }
    bool operator==(const Cliente& other) const { return nome == other.nome; }
};

#endif // CLIENTE_HPP