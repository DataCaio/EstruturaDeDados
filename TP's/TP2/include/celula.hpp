#ifndef CELULA_H
#define CELULA_H

#include "pacote.hpp"

class Celula {
private:
    Pacote pacote;
    Celula *prox;

public:
    Celula() {
        prox = nullptr;
    }

    // Este construtor recebe um Pacote e o armazena.
    Celula(const Pacote& p) : pacote(p), prox(nullptr) {}

    friend class Pilha;
};

#endif