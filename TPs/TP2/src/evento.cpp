#include "evento.hpp"
#include <stdexcept> // Para lançar erros
#include <string>      // Para std::to_string
#include <sstream>     // Para std::stringstream
#include <iomanip> 

// Construtor padrão
Evento::Evento() {
    this->tempo = -1;
    this->tipoEvento = -1;
}
// --- MUDANÇA: Simplifique o operator< ---
bool Evento::operator<(const Evento& outro) const {
    // A única comparação necessária agora é a da chave!
    return this->chave < outro.chave;
}

// O NOVO CONSTRUTOR UNIVERSAL
Evento::Evento(int tempo, int tipo, int arg1, int arg2) {
    this->tempo = tempo;
    this->tipoEvento = tipo;

    if (tipo == 1) { // Chegada de Pacote
        this->dados.id_pacote = arg1;
        this->dados.local = arg2; 
    } else if (tipo == 2) { // Transporte
        this->dados.local = arg1; 
        this->dados.destino = arg2; 
    }
    // Chame o novo método para construir a chave
    gera_chave();
}

void Evento::gera_chave() {
    std::ostringstream oss;
    // Formata o tempo com 7 dígitos para evitar problemas com simulações longas
    oss << std::setfill('0') << std::setw(7) << this->tempo;

    if (this->tipoEvento == 1) { // Evento de Pacote
        // Usa o ID do pacote e o local de chegada para desempate
        oss << std::setfill('0') << std::setw(3) << this->dados.id_pacote;
        oss << std::setfill('0') << std::setw(3) << this->dados.local;
    } else if (this->tipoEvento == 2) { // Evento de Transporte
        // Usa a origem e o destino para desempate
        oss << std::setfill('0') << std::setw(3) << this->dados.local;
        oss << std::setfill('0') << std::setw(3) << this->dados.destino;
    }
    // O tipo do evento é o último critério
    oss << this->tipoEvento; 
    
    this->chave = oss.str();
}
std::string Evento::getChave() const {
    return this->chave;
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