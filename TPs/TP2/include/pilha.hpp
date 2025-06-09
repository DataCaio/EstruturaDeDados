#ifndef PILHA_H
#define PILHA_H

#include "celula.hpp"
#include "pacote.hpp"

class Pilha{
    private:
        Celula* topo;
        int tamanho;
        bool vazia;
    public:
        Pilha();
        virtual ~Pilha();

        void Empilha(Celula *celula);
        Pacote Desempilha();
        void limpa();

    
};

#endif