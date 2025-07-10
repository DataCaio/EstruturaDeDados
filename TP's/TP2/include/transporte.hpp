#ifndef TRANSPORTE_H
#define TRANSPORTE_H

#include "grafo.hpp"

#include <iostream>
#include <fstream>
class Escalonador;

class Transporte {
private:
    Grafo rotas;
public:
    Transporte(int numVertices);
    
    void adicionaRotas(std::ifstream& arquivo);
    void exibirRotas();
    void agendarTransportesIniciais(Escalonador& escalonador, int intervalo);

    Grafo& getGrafo() {
        return rotas;
    }
};

#endif