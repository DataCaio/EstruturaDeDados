#include "transporte.hpp"
#include "evento.hpp"      // Necessário para criar objetos Evento
#include "escalonador.hpp" // Necessário para conhecer a classe Escalonador

#include <iostream>

// Construtor
Transporte::Transporte(int numVertices) : rotas(numVertices) {
}

// Método para ler a matriz de adjacência do grafo interno.
// Ele simplesmente "repasa" a chamada para o objeto Grafo.
void Transporte::adicionaRotas(std::ifstream& arquivo) {
    this->rotas.lerMatriz(arquivo);
}

// Método principal que inicializa os eventos de transporte da simulação.
void Transporte::agendarTransportesIniciais(Escalonador& escalonador, int intervalo) {
    // Pega o número de armazéns a partir do grafo interno.
    int numArmazens = this->rotas.getNumVertices();

    // Itera sobre a metade superior da matriz de adjacência para encontrar cada ligação única.
    for (int i = 0; i < numArmazens; ++i) {
        for (int j = i + 1; j < numArmazens; ++j) {
            
            // Se 'rotas[i][j]' for 1, existe uma ligação física entre os armazéns i e j.
            // Isso assume que a classe Grafo tem o operador[] sobrecarregado.
            if (this->rotas[i][j] == 1) {
                
                // Para cada ligação física, agendamos dois eventos de transporte,
                // um para cada sentido, pois as rotas são bidirecionais.

                
                Evento eventoIda(intervalo, 2, i, j);
                escalonador.insereEvento(eventoIda);
                
                Evento eventoVolta(intervalo, 2, j, i);
                escalonador.insereEvento(eventoVolta);
            }
        }
    }
}