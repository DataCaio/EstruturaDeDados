#include "evento.hpp"

#include <string>
#include <iomanip> // para setfill e setw na funcao gera_chave()
#include <sstream> // para montar a chave

// Construtor padrao
Evento::Evento()
: tempo(-1), idPacote(-1), tipo(-1), origem_id(-1), destino_id(-1) {}

// Construtor
Evento::Evento(int _tempo, int _tipo,
    int _origem_id, int _destino_id, 
    int _idPacote)
        : tempo(_tempo), idPacote(_idPacote), tipo(_tipo),
        origem_id(_origem_id), destino_id(_destino_id) {
    
    gera_chave();

}

/* Sobrecarga de operador de comparacao para tratar
possiveis casos onde eh necessario desempate no
heap que servira de fila de prioridade */
bool Evento::operator<(const Evento& outro) const {
    if (this->chave != outro.chave) {
        return this->chave < outro.chave;
    }
    
    // Criterio de urgencia principal: tempo em que ocorreu
    if (this->tempo != outro.tempo) {
        return this->tempo < outro.tempo;
    }

    // Primeiro criterio de desempate: id do armazem de origem
    if (this->origem_id != outro.origem_id) {
        return this->origem_id < outro.origem_id;
    }

    if (this->destino_id != outro.destino_id) {
        return this->destino_id < outro.destino_id;
    }

    // Criterio de desempate final: id do pacote
    if (this->idPacote != outro.idPacote) {
        return this->idPacote < outro.idPacote;
    }

    return false;
}

void Evento::gera_chave() {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(6) << tempo;

    if (tipo == 1) {
        oss << std::setfill('0') << std::setw(6) << idPacote;
    } else if  (tipo == 2) {
        oss << std::setfill('0') << std::setw(3) << origem_id
            << std::setfill('0') << std::setw(3) << destino_id;
    }

    chave = oss.str();
    chave += std::to_string(tipo);
}
