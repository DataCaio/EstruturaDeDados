#include "evento.hpp"
#include <stdexcept> 

// Construtor padrão
Evento::Evento() {
    this->tempo = -1;
    this->tipo = PACOTE; // Um valor padrão
    this->prioridade = -1;
    this->id_pacote = -1;
    this->origem = -1;
    this->destino = -1;
}

// Construtor para Chegada de Pacote
Evento::Evento(int tempo, int id_pacote, int local_chegada) {
    this->tempo = tempo;
    this->tipo = PACOTE;
    this->id_pacote = id_pacote;
    this->origem = local_chegada; 
    this->destino = local_chegada;

    long long time_part = this->tempo;
    long long package_part = this->id_pacote;
    long long type_part = static_cast<long long>(this->tipo);
    this->prioridade = (time_part * 10000000LL) + (package_part * 10LL) + type_part;
}

// Construtor para Transporte de Pacotes
Evento::Evento(int tempo, int origem_transporte, int destino_transporte, bool is_transporte) {
    this->tempo = tempo;
    this->tipo = TRANSPORTE;
    this->id_pacote = -1; // Não aplicável
    this->origem = origem_transporte;
    this->destino = destino_transporte;

    long long time_part = this->tempo;
    long long origin_part = this->origem;
    long long dest_part = this->destino;
    long long type_part = static_cast<long long>(this->tipo);
    this->prioridade = (time_part * 10000000LL) + (origin_part * 10000LL) + (dest_part * 10LL) + type_part;
}

bool Evento::operator<(const Evento& outro) const {
    return this->prioridade < outro.prioridade;
}

// Implementação dos Getters
int Evento::getTempo() const { return tempo; }

TipoEvento Evento::getTipo() const { return tipo; }

int Evento::getIdPacote() const {
    if (tipo != PACOTE) throw std::logic_error("Chamada invalida: getIdPacote()");
    return id_pacote;
}

int Evento::getLocalChegada() const {
    if (tipo != PACOTE) throw std::logic_error("Chamada invalida: getLocalChegada()");
    return destino;
}

int Evento::getOrigemTransporte() const {
    if (tipo != TRANSPORTE) throw std::logic_error("Chamada invalida: getOrigemTransporte()");
    return origem;
}

int Evento::getDestinoTransporte() const {
    if (tipo != TRANSPORTE) throw std::logic_error("Chamada invalida: getDestinoTransporte()");
    return destino;
}