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
    this->data = new Evento[maxsize];
}

Heap::~Heap() {
    if (data != nullptr) {
        delete[] data;
    }
}

void Heap::Inserir(const Evento& novoEvento) {
    if (tamanho == capacidade) {
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
        return Evento(); 
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

// Métodos Getters
int Heap::GetAncestral(int posicao) { return (posicao - 1) / 2; }
int Heap::GetSucessorEsq(int posicao) { return (posicao * 2) + 1; }
int Heap::GetSucessorDir(int posicao) { return (posicao * 2) + 2; }

void Heap::HeapifyPorCima(int posicao) {
    if (posicao <= 0) return;
    
    int pai = GetAncestral(posicao);
    if (data[posicao] < data[pai]) {
        std::swap(data[posicao], data[pai]);
        HeapifyPorCima(pai);
    }
}

void Heap::HeapifyPorBaixo(int posicao) {
    int esq = GetSucessorEsq(posicao);
    int dir = GetSucessorDir(posicao);
    int menor = posicao;

    if (esq < tamanho && data[esq] < data[menor]) {
        menor = esq;
    }
    
    if (dir < tamanho && data[dir] < data[menor]) {
        menor = dir;
    }

    if (menor != posicao) {
        std::swap(data[posicao], data[menor]);
        HeapifyPorBaixo(menor);
    }
}