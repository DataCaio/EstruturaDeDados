#ifndef PACOTES_HPP
#define PACOTES_HPP

#include <string>
#include "lista.hpp"

class Pacote {
public:
    int id;
    std::string remetente;  
    std::string destinatario; 
    int linhaRG;  
    int linhaUltimo;    

    ListaEncadeada<int> historico; 

    Pacote(int p_id = 0) : id(p_id), linhaRG(-1), linhaUltimo(-1) {}

    bool operator<(const Pacote& other) const { return id < other.id; }
    bool operator>(const Pacote& other) const { return id > other.id; }
    bool operator==(const Pacote& other) const { return id == other.id; }
};

#endif 