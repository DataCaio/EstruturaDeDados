#include "transporte.hpp"
#include "evento.hpp"      // Necessário para criar objetos Evento
#include "escalonador.hpp" // Necessário para conhecer a classe Escalonador

#include <iostream>

// Construtor
Transporte::Transporte(int numVertices) : rotas(numVertices) {
}

// Método para ler a matriz de adjacência do grafo interno.
void Transporte::adicionaRotas(std::ifstream& arquivo) {
    this->rotas.lerMatriz(arquivo);
}

// Método principal que inicializa os eventos de transporte da simulação.
void Transporte::agendarTransportesIniciais(Escalonador& escalonador, int tempoDoTransporte) {
    int numArmazens = this->rotas.getNumVertices();

    for (int i = 0; i < numArmazens; ++i) {
        for (int j = i + 1; j < numArmazens; ++j) {
            
            if (this->rotas[i][j] == 1) {
                
                Evento eventoIda(tempoDoTransporte, i, j, true);
                escalonador.insereEvento(eventoIda);
                
                Evento eventoVolta(tempoDoTransporte, j, i, true);
                escalonador.insereEvento(eventoVolta);
            }
        }
    }
}