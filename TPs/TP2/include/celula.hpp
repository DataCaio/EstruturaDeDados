#ifndef CELULA_H
#define CELULA_H

#include "pacote.hpp"

class Celula{
    private:
        Pacote pacote;
        Celula *prox;
    public:
        Celula(){
            pacote.setChave(-1);
            prox = nullptr;
        }
    friend class Pilha;
};

#endif