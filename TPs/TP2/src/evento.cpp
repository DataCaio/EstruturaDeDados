#include "evento.hpp"
#include <stdexcept> // Para lançar erros

// Construtor padrão
Evento::Evento() {
    this->tempo = -1;
    this->tipoEvento = -1;
}
bool Evento::operator<(const Evento& outro) const {
    if (this->tempo != outro.tempo) {
        return this->tempo < outro.tempo;
    }

    // O tempo é igual, vamos desempatar pela origem (ou ID do pacote)
    int local_a = (this->tipoEvento == 1) ? this->dados.id_pacote : this->dados.local;
    int local_b = (outro.tipoEvento == 1) ? outro.dados.id_pacote : outro.dados.local;

    if (local_a != local_b) {
        return local_a < local_b;
    }

    // Se ainda empatar, desempata pelo destino (ou local de chegada do pacote)
    int destino_a = (this->tipoEvento == 1) ? this->dados.local : this->dados.destino;
    int destino_b = (outro.tipoEvento == 1) ? outro.dados.local : outro.dados.destino;

    if (destino_a != destino_b) {
        return destino_a < destino_b;
    }

    // Como último recurso, desempata pelo tipo do evento
    return this->tipoEvento < outro.tipoEvento;
}

// O NOVO CONSTRUTOR UNIVERSAL
Evento::Evento(int tempo, int tipo, int arg1, int arg2) {
    this->tempo = tempo;
    this->tipoEvento = tipo;

    if (tipo == 1) { // Chegada de Pacote
        this->dados.id_pacote = arg1;
        this->dados.local = arg2; // arg2 é o local_chegada
    } else if (tipo == 2) { // Transporte
        this->dados.local = arg1; // arg1 é a origem
        this->dados.destino = arg2; // arg2 é o destino
    }
}

// Implementação dos Getters
int Evento::getTempo() const {
    return tempo;
}

int Evento::getTipo() const {
    return tipoEvento;
}

// Getters para tipo 1
int Evento::getIdPacote() const {
    if (tipoEvento != 1) throw std::logic_error("Chamada invalida: getIdPacote() so para eventos tipo 1.");
    return dados.id_pacote;
}

int Evento::getLocalChegada() const {
    if (tipoEvento != 1) throw std::logic_error("Chamada invalida: getLocalChegada() so para eventos tipo 1.");
    return dados.local;
}

// Getters para tipo 2
int Evento::getOrigemTransporte() const {
    if (tipoEvento != 2) throw std::logic_error("Chamada invalida: getOrigemTransporte() so para eventos tipo 2.");
    return dados.local;
}

int Evento::getDestinoTransporte() const {
    if (tipoEvento != 2) throw std::logic_error("Chamada invalida: getDestinoTransporte() so para eventos tipo 2.");
    return dados.destino;
}