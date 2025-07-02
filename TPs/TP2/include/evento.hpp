#ifndef EVENTO_H
#define EVENTO_H
#include <string> 

enum TipoEvento {
    PACOTE = 1,
    TRANSPORTE = 2
};

class Evento {
private:
    // A chave de prioridade agora é um 'long long' para o cálculo numérico.
    long long prioridade; 

public:

    int tempo;
    TipoEvento tipo;
    int id_pacote;
    int origem;
    int destino;

    // Construtor padrão
    Evento();

    // Construtor para eventos de PACOTE
    Evento(int tempo, int id_pacote, int local_chegada);

    // Construtor para eventos de TRANSPORTE
    Evento(int tempo, int origem_transporte, int destino_transporte, bool is_transporte);

    // O operador de comparação agora usa a prioridade numérica.
    bool operator<(const Evento& outro) const;
    
    // --- Getters ---
    int getTempo() const;
    TipoEvento getTipo() const;
    int getIdPacote() const;
    int getLocalChegada() const;
    int getOrigemTransporte() const;
    int getDestinoTransporte() const;
};

#endif