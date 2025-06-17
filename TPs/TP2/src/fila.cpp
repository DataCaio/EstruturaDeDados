#include "fila.hpp"
#include <iostream>

Fila::Fila() {
    frente = nullptr;
    tras = nullptr;
}

Fila::~Fila() {
    while (!vazia()) {
        desenfileirar();
    }
}

bool Fila::vazia() {
    return frente == nullptr;
}

void Fila::enfileirar(int item) {
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

int Fila::desenfileirar() {
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