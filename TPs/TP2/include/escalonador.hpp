#ifndef ESCALONADOR_H
#define ESCALONADOR_H
#include "minHeap.hpp"
#include "string"

class Escalonador : public Heap{
    private:
        std::string tempoAtual;
    public:
        Escalonador(int &tempo);
        void inicializa();
        void insereEvento();
        void retiraEvento(); /* possivel troca de tipo */
        void finaliza();

};

#endif