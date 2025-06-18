#ifndef PACOTE_H
#define PACOTE_H

#include <string>
#include "lista.hpp"
#include "grafo.hpp"
#include "fila.hpp"

class Pacote {
private:
    int chave;
    std::string estado;
    int origem;
    int destino;
    int tempoChegada;

    // Rota e posição na rota
    ListaEncadeada rota;
    No* posicao_atual_na_rota;

    // Estatísticas
    int tempoArmazenado;
    int tempoTransportado;

public:
    Pacote();
    Pacote(int tempoChegada, int chave, int origem, int destino);
     Pacote(const Pacote& outro);
    ~Pacote();

    // Métodos de Rota
    void calcularMinhaRota(Grafo& grafo);
    void avancarRota();
    No* getPosicaoAtual() const;
    int getProximoDestinoNaRota() const;
    int getDestinoFinal() const;

    // Getters e Setters de Estado/Info
    int getChave() const;
    void setChave(int ch);
    void alteraEstado(std::string novoEstado);
    std::string getEstado() const;

    int getTempoChegada(){
        return this->tempoChegada;
    }
};

#endif