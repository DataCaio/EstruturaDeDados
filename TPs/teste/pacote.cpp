#include "pacote.hpp"

// Construtor padrao
Pacote::Pacote()
: id(-1), tempoChegada(-1), origem(-1), destino(-1) {
    tempoArmazenado = 0;
    tempoEmTransporte = 0;

    rota = new Lista();
}

// Construtor
Pacote::Pacote(int _tempoChegada, int _id, int _origem, int _destino)
: id(_id), tempoChegada(_tempoChegada), origem(_origem), 
    destino(_destino) {
        tempoArmazenado = 0;
        tempoEmTransporte = 0;

        rota = new Lista();
}

//Construtor de copia
Pacote::Pacote(const Pacote& outro) 
: id(outro.id), 
tempoChegada(outro.tempoChegada), 
origem(outro.origem), destino(outro.destino),
tempoArmazenado(outro.tempoArmazenado), 
tempoEmTransporte(outro.tempoEmTransporte), 
estado(outro.estado)
{
    if (outro.rota) {
        rota = new Lista(*(outro.rota));
    } else {
        rota = nullptr;
    }
}

Pacote& Pacote::operator=(const Pacote& outro) {
    if (this == &outro) {
        return *this;
    }
    delete rota;
    rota = nullptr;

    id = outro.id;
    tempoChegada = outro.tempoChegada;
    origem = outro.origem; destino = outro.destino;
    tempoArmazenado = outro.tempoArmazenado;
    tempoEmTransporte = outro.tempoEmTransporte;
    estado = outro.estado;

    if (outro.rota) {
        rota = new Lista(*(outro.rota));
    } else {
        rota = nullptr;
    }

    return *this;
}

// Destrutor
Pacote::~Pacote() {
    delete rota;
}

// GETTERS E SETTERS
const int Pacote::getId() {
    return id;
}

const int Pacote::getTempoChegada() {
    return tempoChegada;
}

const int Pacote::getOrigem() {
    return origem;
}

const int Pacote::getDestino() {
    return destino;
}

const int Pacote::getTempoArmazenado() {
    return tempoArmazenado;
}

const int Pacote::getTempoEmTransporte() {
    return tempoEmTransporte;
}

const EstadoPacote Pacote::getEstado() {
    return estado;
}

Lista * Pacote::getRota() {
    return rota;
}

void Pacote::setId(int _id) {
    id = _id;
}

void Pacote::setTempoChegada(int novaDataHora) {
    tempoChegada = novaDataHora;
}

void Pacote::setOrigem(int novaOrigem) {
    origem = novaOrigem;
}

void Pacote::setDestino(int novoDestino) {
    destino = novoDestino;
}

void Pacote::setTempoArmazenado(int t) {
    tempoArmazenado = t;
}

void Pacote::setTempoEmTransporte(int t) {
    tempoEmTransporte = t;
}

void Pacote::setEstado(EstadoPacote e) {
    estado = e;
}

void Pacote::setRota(Lista * _rota) {
    rota = _rota;
}
