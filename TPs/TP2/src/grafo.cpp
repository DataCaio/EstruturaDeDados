#include "grafo.hpp"
#include <iostream>

Grafo::Grafo(int vertices){
    this->numVertices = vertices;
    matriz = new int*[this->numVertices];
    for (int i = 0; i < this->numVertices; ++i) {
        matriz[i] = new int[this->numVertices];
    }
}


Grafo::~Grafo() {
    for (int i = 0; i < this->numVertices; ++i) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

void Grafo::lerMatriz() {
    for (int i = 0; i < this->numVertices; ++i) {
        for (int j = 0; j < this->numVertices; ++j) {
            std::cin >> matriz[i][j];
        }
    }
}