#include "armazem.hpp"

// Construtor: Inicializa os membros com valores seguros.
Armazem::Armazem() {
    this->secoes = nullptr;
    this->num_secoes = 0;
}

// Destrutor: Libera a memória alocada para o array de pilhas.
Armazem::~Armazem() {
    // A memória só é deletada se foi alocada.
    if (this->secoes != nullptr) {
        delete[] this->secoes;
    }
}

// Aloca o array de pilhas (seções) com o tamanho correto.
// Deve ser chamado no main depois de criar o array de Armazens.
void Armazem::inicializa(int numTotalArmazens) {
    this->num_secoes = numTotalArmazens;
    this->secoes = new Pilha[this->num_secoes];
}

// Verifica se a pilha para um destino específico não está vazia.
bool Armazem::temPacotesPara(int destino_transporte) const {
    // Checagem de segurança para evitar acesso a índice inválido
    if (destino_transporte < 0 || destino_transporte >= num_secoes) {
        return false;
    }
    // A pilha.hpp deve ter um método Vazia() ou similar
    return !this->secoes[destino_transporte].Vazia();
}

// Armazena um pacote na seção correta.
void Armazem::armazena(Pacote& pacote, int proximo_destino_na_rota) {
    // Checagem de segurança
    if (proximo_destino_na_rota < 0 || proximo_destino_na_rota >= num_secoes) {
        return; // Não armazena em uma seção inválida
    }
    
    // O seu TAD Pilha espera um ponteiro para Celula.
    // Então, criamos uma nova Celula para conter o pacote.
    Celula* nova_celula = new Celula(pacote);
    
    // Empilha a célula na pilha da seção correta.
    this->secoes[proximo_destino_na_rota].Empilha(nova_celula);
}

// Recupera (desempilha) um pacote de uma seção específica.
Pacote Armazem::recupera(int destino_transporte) {
    // Checagem de segurança
    if (!temPacotesPara(destino_transporte)) {
        // Idealmente, lançaria um erro, mas por simplicidade retornamos um pacote vazio.
        return Pacote();
    }
    
    // A pilha.hpp Desempilha e retorna um objeto Pacote.
    return this->secoes[destino_transporte].Desempilha();
}