#ifndef PACOTE_H
#define PACOTE_H

#include "lista.hpp"
#include "grafo.hpp"
#include "fila.hpp"

#include <string>

class Pacote{
    private:
        int chave;
        int alteracoes; /*!!*/
        std::string estado;

        int origem;
        int destino;
        ListaEncadeada rota; // <-- ADICIONE ESTA LINHA
        No* posicao_atual_na_rota; // <-- ADICIONE ESTA LINHA

        int tempoArmazenado;
        int tempoTransportado;
        int tempoChegada;

    public:
        Pacote(int tempoChegada,int chave,int origem, int destino);
        Pacote();
        ~Pacote();    
        
        void calcularMinhaRota(Grafo& grafo); // <-- ADICIONE ESTE MÉTODO
        void avancarRota(); // <-- ADICIONE ESTA LINHA
        No* getPosicaoAtual(); // <-- ADICIONE ESTA LINHA
    
        // Método para obter o próximo destino na rota
        int getProximoDestino();
        int getChave();
        
        void setChave(int chave);

        void acrescentaTempoArmzenado(int tempo);
        int getTempoArmazenado();

        void acrescentaTempoTransportado(int tempo);
        int getTempoTransportado();

        void alteraEstado(std::string novoEstado);
        std::string getEstado();
        
    friend class pilha;
};

#endif