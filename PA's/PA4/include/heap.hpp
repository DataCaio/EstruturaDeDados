#ifndef HEAP_HPP
#define HEAP_HPP

#include "unionFind.hpp"

class Heap {
public:
    Heap(int maxsize);
    ~Heap();

    void Inserir(Aresta x);
    Aresta Remover();
    bool Vazio() const;

private:
    int GetAncestral(int posicao) const;
    int GetSucessorEsq(int posicao) const;
    int GetSucessorDir(int posicao) const;

    int tamanho;
    int capacidade;
    Aresta* data;

    void HeapifyPorBaixo(int posicao);
    void HeapifyPorCima(int posicao);
};

#endif
