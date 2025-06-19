#include "escalonador.hpp"
#include "iostream"

Escalonador::Escalonador() : eventos() { // Chama o construtor padrão de Heap
    this->tempoAtual = 0;
    this->numPacotesEntregues = 0;
    this->totalPacotes = 0;
    this->tempoTotalEmSistema = 0.0;
    this->tempoFinalSimulacao = 0;
}

// O NOVO CONSTRUTOR COM A SOLUÇÃO CORRETA
Escalonador::Escalonador(int capacidade) : eventos(capacidade) { // <-- A MÁGICA ACONTECE AQUI
    this->tempoAtual = 0;
    this->totalPacotes = 0;
    this->numPacotesEntregues = 0;
    this->tempoTotalEmSistema = 0.0;
    this->tempoFinalSimulacao = 0;
}

void Escalonador::inicializa(Pacote* pacotes, int numPacotes, Armazem* armazens, int numArmazens, Transporte& transporte, int latencia, int intervalo, int capacidade, int custoRemocao) {
    this->totalPacotes = numPacotes;
    while (!this->eventos.Vazio()) {
        
        Evento proximo_evento = this->retiraEvento();
        this->tempoAtual = proximo_evento.getTempo();

        // Em src/escalonador.cpp

        // Substitua todo o bloco if (proximo_evento.getTipo() == 2) { ... } por este:
        if (proximo_evento.getTipo() == 2) {
            int origem = proximo_evento.getOrigemTransporte();
            int destino = proximo_evento.getDestinoTransporte();
            int tempo_evento_atual = this->tempoAtual; // Salva o tempo original do evento (ex: 100)

            if (armazens[origem].temPacotesPara(destino)) {
                Pilha pilha_temp;
                int tempo_operacao = tempo_evento_atual;
                int tempo_primeira_remocao = -1;

                // 1. Esvazia a seção, registrando os tempos corretos
                // A lógica da saída esperada indica um custo duplo para o primeiro pacote.
                // Simulamos isso incrementando o tempo antes e dentro do loop.
                tempo_operacao += custoRemocao; // Custo de "acesso" à seção

                while (armazens[origem].temPacotesPara(destino)) {
                    tempo_operacao += custoRemocao; // Custo de remoção do pacote atual

                    if (tempo_primeira_remocao == -1) {
                        tempo_primeira_remocao = tempo_operacao; // Registra o tempo de conclusão da primeira remoção
                    }

                    Pacote pacote_removido = armazens[origem].recupera(destino);
                    printf("%07d pacote %03d removido de %03d na secao %03d\n", tempo_operacao, pacote_removido.getChave(), origem, destino);
                    pilha_temp.Empilha(new Celula(pacote_removido));
                }

                // O tempo de partida é o momento em que a última remoção foi concluída.
                int tempo_partida = tempo_operacao;
                this->tempoAtual = tempo_partida; // Atualiza o relógio global

                // 2. Transportar pacotes da pilha temporária e re-armazenar excedentes
                int pacotes_transportados = 0;
                Pilha pilha_rearmazenar;

                while (!pilha_temp.Vazia()) {
                    if (pacotes_transportados < capacidade) {
                        Pacote p = pilha_temp.Desempilha();
                        printf("%07d pacote %03d em transito de %03d para %03d\n", tempo_partida, p.getChave(), origem, destino);
                        Evento evento_chegada(tempo_partida + latencia, 1, p.getChave(), destino);
                        this->insereEvento(evento_chegada);
                        pacotes_transportados++;
                    } else {
                        pilha_rearmazenar.Empilha(new Celula(pilha_temp.Desempilha()));
                    }
                }

                while(!pilha_rearmazenar.Vazia()){
                    Pacote p = pilha_rearmazenar.Desempilha();
                    armazens[origem].armazena(p, destino);
                    printf("%07d pacote %03d rearmazenado em %03d na secao %03d\n", tempo_partida, p.getChave(), origem, destino);
                }

                // 3. Agenda o próximo evento de transporte relativo ao tempo da PRIMEIRA remoção
                // Em TP2/src/escalonador.cpp
                int tempo_proximo_transporte = tempo_evento_atual + intervalo;          
                Evento proximo_transporte(tempo_proximo_transporte, 2, origem, destino);
                this->insereEvento(proximo_transporte);

            } else if (this->numPacotesEntregues < this->totalPacotes){
                // Se não há pacotes para transportar, apenas reagende o próximo evento para esta rota
                int tempo_proximo_transporte = tempo_evento_atual + intervalo;
                Evento proximo_transporte(tempo_proximo_transporte, 2, origem, destino);
                this->insereEvento(proximo_transporte);
            }
        }
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
                // Se pacote chegou ao destino final
                if (local_chegada == pacote_atual->getDestinoFinal()) {
                    // Registra entrega de pacote
                    pacote_atual->alteraEstado("Entregue");
                    // Exemplo de como gerar a saída formatada:
                    printf("%07d pacote %03d entregue em %03d\n", this->tempoAtual, id_pacote_chegou, local_chegada);
                    
                    this->AtualizaEstatisticas(*pacote_atual);
                } else {
                    // Senão, armazena o pacote na seção para o próximo destino
                    int proximo_destino_na_rota = pacote_atual->getProximoDestinoNaRota();
                    
                    if (proximo_destino_na_rota != -1) {
                        armazens[local_chegada].armazena(*pacote_atual, proximo_destino_na_rota);
                        pacote_atual->alteraEstado("Armazenado");
                        printf("%07d pacote %03d armazenado em %03d na secao %03d\n", this->tempoAtual, id_pacote_chegou, local_chegada, proximo_destino_na_rota);
                        
                        // O pacote chegou e foi armazenado. AGORA avançamos sua rota interna.
                        pacote_atual->avancarRota(); // <<< ESTA LINHA FOI MOVIDA PARA CÁ
                    }
                }
            }
        }
    }

    std::cout << "Fim da simulacao." << std::endl;

}
// Em escalonador.cpp

void Escalonador::insereEvento(Evento evento) {
    // A chamada agora passa o objeto Evento inteiro.
    this->eventos.Inserir(evento); 
}

Evento Escalonador::retiraEvento() {
    // A chamada já retorna um objeto Evento, o que é perfeito.
    Evento proximo = this->eventos.Remover();
    this->tempoAtual = proximo.getTempo();
    return proximo;
}
void Escalonador::finaliza() {
    this->MostraEstatisticas();
}

void Escalonador::AtualizaEstatisticas(Pacote& pacote_entregue) {
    this->numPacotesEntregues++;
    
    int tempoDeVida = this->tempoAtual - pacote_entregue.getTempoChegada();
    this->tempoTotalEmSistema += tempoDeVida;

    // A cada entrega, guardamos o tempo atual como o tempo "oficial" de fim.
    this->tempoFinalSimulacao = this->tempoAtual;
}
void Escalonador::MostraEstatisticas() {
    std::cout << "--- ESTATISTICAS FINAIS ---" << std::endl;
    std::cout << "Tempo total da simulacao: " << this->tempoFinalSimulacao << std::endl;
    std::cout << "Numero de pacotes entregues: " << this->numPacotesEntregues << std::endl;
    
    if (numPacotesEntregues > 0) {
        double tempoMedio = tempoTotalEmSistema / numPacotesEntregues;
        std::cout << "Tempo medio de entrega por pacote: " << tempoMedio << std::endl;
    } else {
        std::cout << "Nenhum pacote foi entregue." << std::endl;
    }
    std::cout << "---------------------------" << std::endl;
}