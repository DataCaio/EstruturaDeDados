#ifndef ESCALONADOR_H
#define ESCALONADOR_H
#include "minHeap.hpp"
#include "evento.hpp"
#include "transporte.hpp"
#include "pacote.hpp"
#include "armazem.hpp" 


class Escalonador{
    private:
        int tempoAtual;
        Heap eventos;

         int numPacotesEntregues;
        double tempoTotalEmSistema; // Usar double para a média


    public:
        Escalonador();
        Escalonador(int capacidade);
        void inicializa(Pacote* pacotes, int numPacotes, Armazem* armazens,
            int numArmazens, Transporte& transporte, 
            int latencia, int intervalo, int capacidade ,int custoRemocao);
        void insereEvento(Evento evento);
        Evento retiraEvento(); /* possivel troca de tipo */
        void finaliza();

        void AtualizaEstatisticas(Pacote& pacote_entregue);
        void MostraEstatisticas();

};

#endif