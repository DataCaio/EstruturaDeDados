#ifndef TRANSPORTE_H
#define TRANSPORTE_H

#include "grafo.hpp"
#include "armazem.hpp"
#include <iostream>

class Transporte {
    private:
        Grafo transportes;
    public:
        Transporte(int numRotas);
        
        void adicionaRotas();
        void exibirRotas();

};

#endif