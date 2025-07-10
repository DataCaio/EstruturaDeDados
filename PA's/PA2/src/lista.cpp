#include "lista.hpp"
#include <iostream>

// Implementação de NoVizinho
NoVizinho::NoVizinho(int v) : valor(v), proximo(nullptr) {}

// Implementação de NoVertice
NoVertice::NoVertice(int i) : id(i), lista_vizinhos(nullptr), proximo(nullptr) {}

// Construtor da lista de adjacência
ListaAdjacencia::ListaAdjacencia() : cabeca(nullptr), totalVertices(0) {}

// Destrutor - libera toda a memória alocada
ListaAdjacencia::~ListaAdjacencia() {
    NoVertice* verticeAtual = cabeca;
    
    while (verticeAtual) {
        NoVizinho* vizinhoAtual = verticeAtual->lista_vizinhos;
        
        // Libera todos os vizinhos do vértice atual
        while (vizinhoAtual) {
            NoVizinho* temp = vizinhoAtual;
            vizinhoAtual = vizinhoAtual->proximo;
            delete temp;
        }
        
        NoVertice* temp = verticeAtual;
        verticeAtual = verticeAtual->proximo;
        delete temp;
    }
}

// Insere um novo vértice na lista
void ListaAdjacencia::inserirVertice() {
    NoVertice* novoVertice = new NoVertice(totalVertices);
    
    if (!cabeca) {
        cabeca = novoVertice;
    } else {
        NoVertice* atual = cabeca;
        while (atual->proximo) {
            atual = atual->proximo;
        }
        atual->proximo = novoVertice;
    }
    totalVertices++;
}

// Insere uma aresta entre dois vértices
void ListaAdjacencia::inserirAresta(int vertice, int vizinho) {
    NoVertice* atual = cabeca;
    
    // Encontra o vértice de origem
    while (atual && atual->id != vertice) {
        atual = atual->proximo;
    }
    
    if (!atual) return; // Vértice não encontrado
    
    NoVizinho* novoVizinho = new NoVizinho(vizinho);
    
    if (!atual->lista_vizinhos) {
        atual->lista_vizinhos = novoVizinho;
    } else {
        NoVizinho* vizinhoAtual = atual->lista_vizinhos;
        while (vizinhoAtual->proximo) {
            vizinhoAtual = vizinhoAtual->proximo;
        }
        vizinhoAtual->proximo = novoVizinho;
    }
}

// Retorna a quantidade total de vértices
int ListaAdjacencia::quantidadeVertices() const {
    return totalVertices;
}

// Retorna a quantidade total de arestas
int ListaAdjacencia::quantidadeArestas() const {
    int somaGraus = 0;
    
    for (NoVertice* vertice = cabeca; vertice; vertice = vertice->proximo) {
        for (NoVizinho* vizinho = vertice->lista_vizinhos; vizinho; vizinho = vizinho->proximo) {
            somaGraus++;
        }
    }
    
    // Cada aresta é contada duas vezes (ida e volta)
    return somaGraus / 2;
}

// Retorna o grau mínimo do grafo
int ListaAdjacencia::grauMinimo() const {
    if (!cabeca) return 0;
    
    int grauMinimo = INT_MAX;
    
    for (NoVertice* vertice = cabeca; vertice; vertice = vertice->proximo) {
        int grau = 0;
        
        for (NoVizinho* vizinho = vertice->lista_vizinhos; vizinho; vizinho = vizinho->proximo) {
            grau++;
        }
        
        if (grau < grauMinimo) {
            grauMinimo = grau;
        }
    }
    
    return grauMinimo;
}

// Retorna o grau máximo do grafo
int ListaAdjacencia::grauMaximo() const {
    int grauMaximo = 0;
    
    for (NoVertice* vertice = cabeca; vertice; vertice = vertice->proximo) {
        int grau = 0;
        
        for (NoVizinho* vizinho = vertice->lista_vizinhos; vizinho; vizinho = vizinho->proximo) {
            grau++;
        }
        
        if (grau > grauMaximo) {
            grauMaximo = grau;
        }
    }
    
    return grauMaximo;
}

// Imprime todos os vizinhos de um vértice
void ListaAdjacencia::imprimirVizinhos(int vertice) const {
    NoVertice* atual = cabeca;
    
    // Encontra o vértice
    while (atual && atual->id != vertice) {
        atual = atual->proximo;
    }
    
    if (!atual) {
        std::cout << "\n";
        return;
    }
    
    // Imprime todos os vizinhos
    for (NoVizinho* vizinho = atual->lista_vizinhos; vizinho; vizinho = vizinho->proximo) {
        std::cout << vizinho->valor;
        if (vizinho->proximo) std::cout << " ";
    }
    std::cout << "\n";
}