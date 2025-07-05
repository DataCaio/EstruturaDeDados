#ifndef LISTA_HPP
#define LISTA_HPP

#include <iostream>
#include <stdexcept>

template <typename T>
struct No {
    T dado;
    No* proximo;

    No(const T& valor) : dado(valor), proximo(nullptr) {}
};

template <typename T>
class ListaEncadeada {
private:
    No<T>* cabeca;
    No<T>* cauda; // <-- NOVO: Ponteiro para o último nó
    int tamanho;

    // Função auxiliar para limpar a lista (evita duplicação de código)
    void limpar() {
        No<T>* atual = cabeca;
        while (atual) {
            No<T>* proximo_no = atual->proximo;
            delete atual;
            atual = proximo_no;
        }
        cabeca = cauda = nullptr;
        tamanho = 0;
    }

public:
    ListaEncadeada() : cabeca(nullptr), cauda(nullptr), tamanho(0) {}

    ListaEncadeada(const ListaEncadeada& outra) : cabeca(nullptr), cauda(nullptr), tamanho(0) {
        No<T>* atual_outro = outra.cabeca;
        while (atual_outro) {
            adicionaNoFim(atual_outro->dado);
            atual_outro = atual_outro->proximo;
        }
    }

    ~ListaEncadeada() {
        limpar();
    }

    void insereNoInicio(const T& valor) {
        No<T>* novo_no = new No<T>(valor);
        novo_no->proximo = cabeca;
        cabeca = novo_no;
        if (cauda == nullptr) { // Se a lista estava vazia
            cauda = novo_no;
        }
        tamanho++;
    }

    void adicionaNoFim(const T& valor) {
        No<T>* novo_no = new No<T>(valor);
        if (cabeca == nullptr) {
            cabeca = cauda = novo_no;
        } else {
            cauda->proximo = novo_no;
            cauda = novo_no;
        }
        tamanho++;
    }

    T getElemento(int index) const {
        if (index < 0 || index >= tamanho) {
            throw std::out_of_range("Índice fora dos limites da lista.");
        }
        No<T>* atual = cabeca;
        for (int i = 0; i < index; ++i) {
            atual = atual->proximo;
        }
        return atual->dado;
    }

    int size() const {
        return tamanho;
    }

    bool contains(const T& valor) const {
        No<T>* atual = cabeca;
        while (atual) {
            if (atual->dado == valor) {
                return true;
            }
            atual = atual->proximo;
        }
        return false;
    }
    
    // <-- NOVO: Método para permitir iteração externa eficiente
    No<T>* getCabeca(){
        return cabeca;
    }

    ListaEncadeada& operator=(const ListaEncadeada& outra) {
        if (this != &outra) {
            limpar(); // Limpa a lista atual
            No<T>* atual_outro = outra.cabeca;
            while (atual_outro) {
                adicionaNoFim(atual_outro->dado);
                atual_outro = atual_outro->proximo;
            }
        }
        return *this;
    }
};

#endif