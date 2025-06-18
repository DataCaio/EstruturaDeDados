#ifndef HEAP_HPP
#define HEAP_HPP

#include "evento.hpp" // <-- MUDANÇA: Incluir a definição de Evento
#include <utility>    // Para std::swap

class Heap {
public:
    Heap();
    Heap(int maxsize);
    ~Heap();

    void Inserir(const Evento& novoEvento); // <-- MUDANÇA: Recebe um Evento
    Evento Remover();                     // <-- MUDANÇA: Retorna um Evento
    bool Vazio();

private:
    int GetAncestral(int posicao);
    int GetSucessorEsq(int posicao);
    int GetSucessorDir(int posicao);

    int tamanho;
    int capacidade; // Adicionado para segurança
    Evento* data;  // <-- MUDANÇA: O array agora é de Eventos

    void HeapifyPorBaixo(int posicao);
    void HeapifyPorCima(int posicao);
};

#endif