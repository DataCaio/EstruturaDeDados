#include "graph.hpp"

Grafo::Grafo() {}
Grafo::~Grafo() {}

void Grafo::InsereVertice() {
    vertices.inserirVertice();
}

void Grafo::InsereAresta(int v, int w) {
    vertices.inserirAresta(v, w);
}

int Grafo::QuantidadeVertices() {
    return vertices.quantidadeVertices();
}

int Grafo::QuantidadeArestas() {
    return vertices.quantidadeArestas();
}

int Grafo::GrauMinimo() {
    return vertices.grauMinimo();
}

int Grafo::GrauMaximo() {
    return vertices.grauMaximo();
}

void Grafo::ImprimeVizinhos(int v) {
    vertices.imprimirVizinhos(v);
}

