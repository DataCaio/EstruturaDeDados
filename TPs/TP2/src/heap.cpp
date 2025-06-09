#include "minHeap.hpp"
#include <iostream>

Heap::Heap(int maxsize) {
    tamanho = 0;
    capacidade = maxsize;
    data = new Aresta[capacidade];
}

Heap::~Heap() {
    delete[] data;
}

void Heap::Inserir(Aresta x) {
    if (tamanho == capacidade) {
        std::cerr << "Heap cheio! Não é possível inserir mais elementos.\n";
        return;
    }

    data[tamanho] = x;
    HeapifyPorCima(tamanho);
    tamanho++;
}

Aresta Heap::Remover() {
    if (Vazio()) {
        std::cerr << "Heap vazio! Nada para remover.\n";
        return Aresta{-1, -1, -1}; // valor sentinel ou lance exceção
    }

    Aresta raiz = data[0];
    data[0] = data[tamanho - 1];
    tamanho--;
    HeapifyPorBaixo(0);
    return raiz;
}

bool Heap::Vazio() const {
    return (tamanho == 0);
}

int Heap::GetAncestral(int posicao) const {
    return (posicao - 1) / 2;
}

int Heap::GetSucessorEsq(int posicao) const {
    return 2 * posicao + 1;
}

int Heap::GetSucessorDir(int posicao) const {
    return 2 * posicao + 2;
}

void Heap::HeapifyPorCima(int posicao) {
    while (posicao > 0) {
        int pai = GetAncestral(posicao);
        if (data[posicao].custo < data[pai].custo) {
            std::swap(data[posicao], data[pai]);
            posicao = pai;
        } else {
            break;
        }
    }
}

void Heap::HeapifyPorBaixo(int posicao) {
    while (true) {
        int menor = posicao;
        int esq = GetSucessorEsq(posicao);
        int dir = GetSucessorDir(posicao);

        if (esq < tamanho && data[esq].custo < data[menor].custo) {
            menor = esq;
        }
        if (dir < tamanho && data[dir].custo < data[menor].custo) {
            menor = dir;
        }

        if (menor != posicao) {
            std::swap(data[posicao], data[menor]);
            posicao = menor;
        } else {
            break;
        }
    }