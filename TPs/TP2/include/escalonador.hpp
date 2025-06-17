#ifndef ESCALONADOR_H
#define ESCALONADOR_H
#include "minHeap.hpp"
#include "evento.hpp"


class Escalonador{
    private:
        int tempoAtual;
        Heap eventos;

    public:
        Escalonador();
        void inicializa();
        void insereEvento(Evento evento);
        Evento retiraEvento(); /* possivel troca de tipo */
        void finaliza();

        void AtualizaEstatisticas();
        void MostraEstatisticas();

};

#endif