#include "transporte.hpp"
#include "iostream"

void Transporte::agendarTransportesIniciais(Escalonador& escalonador, int intervalo) {
    int numArmazens = this->rotas.getNumVertices();;

    for (int i = 0; i < numArmazens; ++i) {
        for (int j = i + 1; j < numArmazens; ++j) {
            
            // Acessa seu próprio grafo interno
            if (this->rotas[i][j] == 1) {
                
                // Cria e insere o evento de ida no escalonador
                Evento eventoIda(/* tempo=intervalo, tipo=2, origem=i, destino=j */);
                escalonador.insereEvento(eventoIda);

                // Cria e insere o evento de volta no escalonador
                Evento eventoVolta(/* tempo=intervalo, tipo=2, origem=j, destino=i */);
                escalonador.insereEvento(eventoVolta);
            }
        }
    }
}