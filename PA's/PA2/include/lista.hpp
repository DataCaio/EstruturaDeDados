#ifndef LISTA_ADJACENCIA_HPP
#define LISTA_ADJACENCIA_HPP

#include <climits>

// Nó que representa um vizinho na lista de adjacência
struct NoVizinho {
    int valor;          // Valor do vértice vizinho
    NoVizinho* proximo; // Próximo nó na lista
    NoVizinho(int v);   // Construtor
};

// Nó que representa um vértice no grafo
struct NoVertice {
    int id;             // Identificador do vértice
    NoVizinho* lista_vizinhos; // Lista de vizinhos
    NoVertice* proximo; // Próximo vértice na lista
    NoVertice(int i);   // Construtor
};

class ListaAdjacencia {
public:
    ListaAdjacencia();
    ~ListaAdjacencia();

    // Operações básicas
    void inserirVertice();
    void inserirAresta(int vertice, int vizinho);

    // Consultas
    int quantidadeVertices() const;
    int quantidadeArestas() const;
    int grauMinimo() const;
    int grauMaximo() const;
    void imprimirVizinhos(int vertice) const;

private:
    NoVertice* cabeca;   // Primeiro vértice da lista
    int totalVertices;   // Contador de vértices
};

#endif // LISTA_ADJACENCIA_HPP