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
    int tamanho;

public:
    ListaEncadeada() : cabeca(nullptr), tamanho(0) {}

    // Construtor de cópia (deep copy)
    ListaEncadeada(const ListaEncadeada& outra) : cabeca(nullptr), tamanho(0) {
        if (!outra.cabeca) return;

        No<T>* atual_outro = outra.cabeca;
        No<T>* ultimo_novo = nullptr;

        // Copia primeiro nó
        cabeca = new No<T>(atual_outro->dado);
        tamanho++;
        ultimo_novo = cabeca;
        atual_outro = atual_outro->proximo;

        // Copia restante
        while (atual_outro) {
            ultimo_novo->proximo = new No<T>(atual_outro->dado);
            ultimo_novo = ultimo_novo->proximo;
            tamanho++;
            atual_outro = atual_outro->proximo;
        }
    }

    ~ListaEncadeada() {
        No<T>* atual = cabeca;
        while (atual) {
            No<T>* proximo_no = atual->proximo;
            delete atual;
            atual = proximo_no;
        }
        cabeca = nullptr; 
        tamanho = 0;    
    }

    void insereNoInicio(const T& valor) {
        No<T>* novo_no = new No<T>(valor);
        novo_no->proximo = cabeca;
        cabeca = novo_no;
        tamanho++;
    }

    void adicionaNoFim(const T& valor) {
        No<T>* novo_no = new No<T>(valor);
        if (cabeca == nullptr) {
            cabeca = novo_no;
        } else {
            No<T>* atual = cabeca;
            while (atual->proximo != nullptr) {
                atual = atual->proximo;
            }
            atual->proximo = novo_no;
        }
        tamanho++;
    }

    // Retorna o elemento em um índice específico
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

    // Retorna o número de elementos na lista (renomeado de getTamanho)
    int size() const {
        return tamanho;
    }

    // Verifica se a lista contém um determinado elemento
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

    // Opcional: Operador de atribuição de cópia para deep copy
    ListaEncadeada& operator=(const ListaEncadeada& outra) {
        if (this == &outra) {
            return *this; // Handle self-assignment
        }

        // Clean up current list
        No<T>* atual = cabeca;
        while (atual) {
            No<T>* proximo_no = atual->proximo;
            delete atual;
            atual = proximo_no;
        }
        cabeca = nullptr;
        tamanho = 0;

        // Copy elements from the other list
        if (!outra.cabeca) return *this;

        No<T>* atual_outro = outra.cabeca;
        No<T>* ultimo_novo = nullptr;

        cabeca = new No<T>(atual_outro->dado);
        tamanho++;
        ultimo_novo = cabeca;
        atual_outro = atual_outro->proximo;

        while (atual_outro) {
            ultimo_novo->proximo = new No<T>(atual_outro->dado);
            ultimo_novo = ultimo_novo->proximo;
            tamanho++;
            atual_outro = atual_outro->proximo;
        }
        return *this;
    }
};

#endif