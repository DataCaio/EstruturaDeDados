#ifndef ESCALONADOR_H
#define ESCALONADOR_H
#include "minHeap.hpp"
#include "evento.hpp"
#include "transporte.hpp"
#include "pacote.hpp"


class Escalonador{
    private:
        int tempoAtual;
        Heap eventos;

    public:
        Escalonador();
        void inicializa(Pacote* pacotes, int numPacotes, Armazem* armazens,
            int numArmazens, Transporte& transporte, 
            int latencia, int intervalo, int capacidade);
        void insereEvento(Evento evento);
        Evento retiraEvento(); /* possivel troca de tipo */
        void finaliza();

        void AtualizaEstatisticas();
        void MostraEstatisticas();

};

#endif