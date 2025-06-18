#include "minHeap.hpp"
#include <iostream>
#include <utility> // Para std::swap

Heap::Heap() {
    this->tamanho = 0;
    this->capacidade = 0;
    this->data = nullptr;
}

Heap::Heap(int maxsize) {
    this->tamanho = 0;
    this->capacidade = maxsize;
    this->data = new Evento[maxsize]; // <-- MUDANÇA: Aloca um array de Eventos
}

Heap::~Heap() {
    if (data != nullptr) {
        delete[] data;
    }
}

void Heap::Inserir(const Evento& novoEvento) {
    if (tamanho == capacidade) {
        // O ideal seria redimensionar o array, mas por enquanto vamos evitar estourar.
        std::cerr << "Erro: Heap cheio!" << std::endl;
        return;
    }
    data[tamanho] = novoEvento;
    HeapifyPorCima(tamanho);
    tamanho++;
}

Evento Heap::Remover() {
    if (Vazio()) {
        std::cerr << "Erro: Tentando remover de um Heap vazio!" << std::endl;
        return Evento(); // <-- MUDANÇA: Retorna um evento padrão/vazio em caso de erro.
    }
    
    Evento raiz = data[0];
    data[0] = data[tamanho - 1];
    tamanho--;
    if (!Vazio()) {
        HeapifyPorBaixo(0);
    }
    return raiz;
}

bool Heap::Vazio() {
    return (tamanho == 0);
}

// Métodos GetAncestral, GetSucessorEsq, GetSucessorDir permanecem os mesmos
int Heap::GetAncestral(int posicao) { return (posicao - 1) / 2; }
int Heap::GetSucessorEsq(int posicao) { return (posicao * 2) + 1; }
int Heap::GetSucessorDir(int posicao) { return (posicao * 2) + 2; }

void Heap::HeapifyPorCima(int posicao) {
    if (posicao <= 0) return;
    
    int pai = GetAncestral(posicao);
    // <-- MUDANÇA: Comparamos os tempos dos eventos, não os objetos inteiros.
    if (data[posicao].getTempo() < data[pai].getTempo()) {
        std::swap(data[posicao], data[pai]);
        HeapifyPorCima(pai); // Chamada recursiva ou laço while
    }
}

void Heap::HeapifyPorBaixo(int posicao) {
    int esq = GetSucessorEsq(posicao);
    int dir = GetSucessorDir(posicao);
    int menor = posicao;

    // <-- MUDANÇA: Compara o tempo do filho esquerdo com o tempo do menor atual.
    if (esq < tamanho && data[esq].getTempo() < data[menor].getTempo()) {
        menor = esq;
    }
    
    // <-- MUDANÇA: Compara o tempo do filho direito com o tempo do menor atual.
    if (dir < tamanho && data[dir].getTempo() < data[menor].getTempo()) {
        menor = dir;
    }

    if (menor != posicao) {
        std::swap(data[posicao], data[menor]);
        HeapifyPorBaixo(menor); // Chamada recursiva ou laço while
    }
}