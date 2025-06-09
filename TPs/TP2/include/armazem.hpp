#ifndef ARMAZEM_H
#define ARMAZEM_H

#include "pilha.hpp"
#include <iostream>

class Armazem{
    private:
        Pilha pacotes;
    public:
        Armazem();
        ~Armazem();

        void armazena(Pacote pacote);
        Pacote recupera();

};

#endif