#include "escalonador.hpp"
#include "iostream"

Escalonador::Escalonador(){
    this->tempoAtual = 0;
}

void Escalonador::inicializa(Pacote* pacotes, int numPacotes, Armazem* armazens, int numArmazens, Transporte& transporte, int latencia, int intervalo, int capacidade) {
    
    while (!this->eventos.Vazio()) {
        
        Evento proximo_evento = this->retiraEvento();
        this->tempoAtual = proximo_evento.getTempo();

        // Se evento é transporte
        if (proximo_evento.getTipo() == 2) {
            
            int origem = proximo_evento.getOrigemTransporte();
            int destino = proximo_evento.getDestinoTransporte();

            // Se há pacotes na seção associada
            if (armazens[origem].temPacotesPara(destino)) {
                
                // Remove os pacotes até a capacidade do transporte
                for (int i = 0; i < capacidade; i++) {
                    
                    if (!armazens[origem].temPacotesPara(destino)) {
                        break; 
                    }

                    // NOTA: A lógica de remoção pode ser mais complexa aqui.
                    // O enunciado menciona 'custoRemocao' e 'rearmazenado'.
                    // Cada 'recupera' pode consumir tempo (this->tempoAtual += custoRemocao).
                    Pacote pacote_transportado = armazens[origem].recupera(destino);
                    
                    // Escalonar a chegada do pacote no próximo armazém
                    int tempo_chegada_destino = this->tempoAtual + latencia;
                    Evento evento_chegada(tempo_chegada_destino, 1, pacote_transportado.getChave(), destino);
                    this->insereEvento(evento_chegada);
                }
            }
            
            // Escalonar o próximo evento de transporte para esta rota
            int tempo_proximo_transporte = this->tempoAtual + intervalo;
            Evento proximo_transporte(tempo_proximo_transporte, 2, origem, destino);
            this->insereEvento(proximo_transporte);

        } 
        // Se evento é chegada de pacotes
        else if (proximo_evento.getTipo() == 1) {
            
            int id_pacote_chegou = proximo_evento.getIdPacote();
            int local_chegada = proximo_evento.getLocalChegada();

            Pacote* pacote_atual = nullptr;
            for (int i = 0; i < numPacotes; i++) {
                if (pacotes[i].getChave() == id_pacote_chegou) {
                    pacote_atual = &pacotes[i];
                    break;
                }
            }

            if (pacote_atual != nullptr) {
                // <-- CORREÇÃO IMPORTANTE AQUI ---
                // O pacote avançou um passo na sua rota, então atualizamos sua posição interna.
                pacote_atual->avancarRota();

                // Se pacote chegou ao destino final
                if (local_chegada == pacote_atual->getDestinoFinal()) {
                    // Registra entrega de pacote
                    pacote_atual->alteraEstado("Entregue");
                    // Exemplo de como gerar a saída formatada:
                    printf("%07d pacote %03d entregue em %03d\n", this->tempoAtual, id_pacote_chegou, local_chegada);
                
                } else {
                    // Senão, armazena o pacote na seção para o próximo destino
                    int proximo_destino_na_rota = pacote_atual->getProximoDestinoNaRota();
                    
                    if (proximo_destino_na_rota != -1) {
                        armazens[local_chegada].armazena(*pacote_atual, proximo_destino_na_rota);
                        pacote_atual->alteraEstado("Armazenado");
                        printf("%07d pacote %03d armazenado em %03d na secao %03d\n", this->tempoAtual, id_pacote_chegou, local_chegada, proximo_destino_na_rota);
                    }
                }
            }
        }
    }

    std::cout << "Fim da simulacao." << std::endl;

}
