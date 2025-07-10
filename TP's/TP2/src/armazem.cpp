#include "armazem.hpp"

// Construtor: Inicializa os membros com valores seguros.
Armazem::Armazem() {
    this->secoes = nullptr;
    this->num_secoes = 0;
}

// Destrutor: Libera a memória alocada para o array de pilhas.
Armazem::~Armazem() {
    if (this->secoes != nullptr) {
        delete[] this->secoes;
    }
}

// Aloca o array de pilhas (seções) com o tamanho correto.
void Armazem::inicializa(int numTotalArmazens) {
    this->num_secoes = numTotalArmazens;
    this->secoes = new Pilha[this->num_secoes];
}

// Verifica se a pilha para um destino específico não está vazia.
bool Armazem::temPacotesPara(int destino_transporte) const {
    if (destino_transporte < 0 || destino_transporte >= num_secoes) {
        return false;
    }
    return !this->secoes[destino_transporte].Vazia();
}

// Armazena um pacote na seção correta.
void Armazem::armazena(Pacote& pacote, int proximo_destino_na_rota) {
    if (proximo_destino_na_rota < 0 || proximo_destino_na_rota >= num_secoes) {
        return; 
    }
    
    Celula* nova_celula = new Celula(pacote);
    
    // Empilha a célula na pilha da seção correta.
    this->secoes[proximo_destino_na_rota].Empilha(nova_celula);
}

// Recupera (desempilha) um pacote de uma seção específica.
Pacote Armazem::recupera(int destino_transporte) {
    if (!temPacotesPara(destino_transporte)) {
        // Idealmente, lançaria um erro, mas por simplicidade retornamos um pacote vazio.
        return Pacote();
    }
    
    // A pilha.hpp Desempilha e retorna um objeto Pacote.
    return this->secoes[destino_transporte].Desempilha();
}