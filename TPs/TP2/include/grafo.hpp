#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>

class Grafo {
private:
    int** matriz;
    int numVertices;

public:
    Grafo(int vertices);
    ~Grafo();

    int* operator[](int i); // Supondo que você manteve a sobrecarga
    void lerMatriz();

    // --- CORREÇÃO ADICIONADA AQUI ---
    // Permite que outras classes consultem o número de vértices de forma segura.
    int getNumVertices() const {
        return numVertices;
    }
};

#endif