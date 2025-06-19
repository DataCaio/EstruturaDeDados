#ifndef FILA_H
#define FILA_H

#include <iostream> // Adicionado para std::cerr

// Nó específico para a Fila
struct NoFila {
    int item;
    NoFila* proximo;
};

class Fila {
private:
    NoFila* frente;
    NoFila* tras;

public:
    Fila() {
        frente = nullptr;
        tras = nullptr;
    }

    ~Fila() {
        while (!vazia()) {
            desenfileirar();
        }
    }

    bool vazia() {
        return frente == nullptr;
    }

    void enfileirar(int item) {
        NoFila* novo_no = new NoFila();
        novo_no->item = item;
        novo_no->proximo = nullptr;
        if (vazia()) {
            frente = novo_no;
            tras = novo_no;
        } else {
            tras->proximo = novo_no;
            tras = novo_no;
        }
    }

    int desenfileirar() {
        if (vazia()) {
            std::cerr << "Erro: Fila vazia!" << std::endl;
            return -1; // Ou lançar uma exceção
        }
        NoFila* temp = frente;
        int item_frente = temp->item;
        frente = frente->proximo;
        if (frente == nullptr) {
            tras = nullptr; // A fila ficou vazia
        }
        delete temp;
        return item_frente;
    }
};

#endif