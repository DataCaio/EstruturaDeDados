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

    int* operator[](int i); 
    void lerMatriz(std::ifstream& arquivo);
    
    int getNumVertices() const {
        return numVertices;
    }
};

#endif