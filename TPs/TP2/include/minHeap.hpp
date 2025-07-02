#ifndef HEAP_HPP
#define HEAP_HPP

#include "evento.hpp" 
#include <utility>    // Para std::swap

class Heap {
public:
    Heap();
    Heap(int maxsize);
    ~Heap();

    void Inserir(const Evento& novoEvento); 
    Evento Remover();
    bool Vazio();

private:
    int GetAncestral(int posicao);
    int GetSucessorEsq(int posicao);
    int GetSucessorDir(int posicao);

    int tamanho;
    int capacidade; 
    Evento* data;

    void HeapifyPorBaixo(int posicao);
    void HeapifyPorCima(int posicao);
};

#endif