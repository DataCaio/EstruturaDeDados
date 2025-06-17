#include "lista.hpp"
#include "iostream"

// Implementação dos métodos da ListaEncadeada
ListaEncadeada::ListaEncadeada() {
    cabeca = nullptr;
    tamanho = 0;
}

ListaEncadeada::~ListaEncadeada() {
    No* atual = cabeca;
    while (atual != nullptr) {
        No* proximo_no = atual->proximo;
        delete atual;
        atual = proximo_no;
    }
}

void ListaEncadeada::insereNoInicio(int id_armazem) {
    No* novo_no = new No(id_armazem);
    novo_no->proximo = cabeca;
    cabeca = novo_no;
    tamanho++;
}

void ListaEncadeada::exibir() {
    No* atual = cabeca;
    while (atual != nullptr) {
        std::cout << atual->id_armazem << " -> ";
        atual = atual->proximo;
    }
    std::cout << "FIM\n";
}

No* ListaEncadeada::getCabeca() {
    return cabeca;
}