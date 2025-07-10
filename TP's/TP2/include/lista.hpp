#ifndef LISTA_H
#define LISTA_H

#include "iostream" 

// O nó da lista, que armazena o ID de um armazém
struct No {
    int id_armazem;
    No* proximo;

    No(int id) : id_armazem(id), proximo(nullptr) {}
};


// A classe que gerencia a lista de nós
class ListaEncadeada {
private:
    No* cabeca;
    int tamanho;

public:
    ListaEncadeada() {
        cabeca = nullptr;
        tamanho = 0;
    }

    ListaEncadeada(const ListaEncadeada& outra) {
        this->cabeca = nullptr;
        this->tamanho = 0;
        
        if (outra.cabeca == nullptr) {
            return; 
        }

        No* atual_outro = outra.cabeca;
        No* ultimo_novo = nullptr;

        // Copia o primeiro nó
        this->cabeca = new No(atual_outro->id_armazem);
        this->tamanho++;
        ultimo_novo = this->cabeca;
        atual_outro = atual_outro->proximo;

        // Copia o resto da lista
        while (atual_outro != nullptr) {
            ultimo_novo->proximo = new No(atual_outro->id_armazem);
            ultimo_novo = ultimo_novo->proximo;
            this->tamanho++;
            atual_outro = atual_outro->proximo;
        }
    }

    ~ListaEncadeada() {
        No* atual = cabeca;
        while (atual != nullptr) {
            No* proximo_no = atual->proximo;
            delete atual;
            atual = proximo_no;
        }
    }

    void insereNoInicio(int id_armazem) {
        No* novo_no = new No(id_armazem);
        novo_no->proximo = cabeca;
        cabeca = novo_no;
        tamanho++;
    }

    void exibir() {
        No* atual = cabeca;
        while (atual != nullptr) {
            std::cout << atual->id_armazem << " -> ";
            atual = atual->proximo;
        }
        std::cout << "FIM\n";
    }
    
    No* getCabeca() const {
        return cabeca;
    }

};

#endif