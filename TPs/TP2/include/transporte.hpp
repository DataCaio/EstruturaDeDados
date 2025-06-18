#ifndef TRANSPORTE_H
#define TRANSPORTE_H

#include "grafo.hpp"
#include "escalonador.hpp"

#include <iostream>

class Transporte {
private:
    Grafo rotas;
public:
    Transporte(int numVertices);
    
    void adicionaRotas();
    void exibirRotas();
    void agendarTransportesIniciais(Escalonador& escalonador, int intervalo);

    Grafo& getGrafo() {
        return rotas;
    }
};

#endif