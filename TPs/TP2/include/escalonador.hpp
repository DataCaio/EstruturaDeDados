#ifndef ESCALONADOR_H
#define ESCALONADOR_H
#include "minHeap.hpp"

class Escalonador : public Heap{
    private:
        int* tempo;
    public:
        Escalonador(int &tempo);
        void inicializa();
        void insereEvento();
        void retiraEvento(); /* possivel troca de tipo */
        void finaliza();

};

#endif