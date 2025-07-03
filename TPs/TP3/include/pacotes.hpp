#ifndef PACOTES_HPP
#define PACOTES_HPP

#include <string>
#include "lista.hpp" // Your custom linked list

class Pacote {
public:
    int id;
    std::string remetente;  // From RG event
    std::string destinatario; // From RG event
    int linhaRG;            // Index of the RG event in the central 'eventos' vector
    int linhaUltimo;        // Index of the latest event in the central 'eventos' vector

    ListaEncadeada<int> historico; // Stores indices (int) of events in the main 'eventos' vector

    Pacote(int p_id = 0) : id(p_id), linhaRG(-1), linhaUltimo(-1) {}

    // Comparison operators for AVLTree key (int id)
    bool operator<(const Pacote& other) const { return id < other.id; }
    bool operator>(const Pacote& other) const { return id > other.id; }
    bool operator==(const Pacote& other) const { return id == other.id; }
};

#endif // PACOTES_HPP