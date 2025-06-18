#include "escalonador.hpp"
#include "iostream"

Escalonador::Escalonador(){
    this->tempoAtual = 0;
}

void Escalonador::inicializa(Pacote* pacotes, int numPacotes, Armazem* armazens, int numArmazens,
     Transporte& transporte, int latencia, int intervalo, int capacidade) {
    
    // O loop 'while' vive aqui, isolado do main.
    while (!this->eventos.Vazio()) {
        Evento proximo_evento = this->retiraEvento();
        this->tempoAtual = proximo_evento.getTempo();

        if (proximo_evento.getTipo() == 2) { // Transporte
            
            // --- Lógica para Transporte ---
            // Usa os parâmetros 'latencia', 'intervalo', 'capacidade', etc.
            int origem = proximo_evento.getOrigem();
            int destino = proximo_evento.getDestino();

            if(armazens[origem])
            // Lógica para remover pacotes do armazem[origem]...
            // Para cada pacote removido:
            //   Evento chegada(this->tempoAtual + latencia, 1, id_pacote_removido);
            //   this->insereEvento(chegada);

            // Agende o próximo transporte para esta rota:
            // Evento proximo_transporte(this->tempoAtual + intervalo, 2, ...);
            // this->insereEvento(proximo_transporte);

            // Chegada de Pacote
            
            // --- Lógica para Chegada de Pacote ---
            // Usa os parâmetros 'pacotes' e 'armazens' que foram passados.
            int id_pacote = proximo_evento.getIdPacote();
            // Lógica para encontrar o pacote no array 'pacotes' com o id_pacote
            // Lógica para encontrar o armazém de chegada
            // Se for destino final, etc...

        } else if (proximo_evento.getTipo() == 1) { // Chegada de Pacote
            
            // --- Lógica para Chegada de Pacote ---
            // Usa os parâmetros 'pacotes' e 'armazens' que foram passados.
            int id_pacote = proximo_evento.getIdPacote();
            // Lógica para encontrar o pacote no array 'pacotes' com o id_pacote
            // Lógica para encontrar o armazém de chegada
            // Se for destino final, etc... + intervalo, 2, ...);
            // this->insereEvento(proximo_transporte);
        }
    }

    std::cout << "Fim da simulacao." << std::endl;
}