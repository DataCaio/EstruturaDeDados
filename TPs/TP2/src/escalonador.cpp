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

        if (proximo_evento.getTipo() == 2) { // Evento de Transporte
            int origem = proximo_evento.getOrigemTransporte();
            int destino = proximo_evento.getDestinoTransporte();
            int tempo_inicio_evento = this->tempoAtual;

            if (armazens[origem].temPacotesPara(destino)) {
                                // --- INÍCIO DA LÓGICA ADAPTADA DO CÓDIGO FUNCIONAL ---

                // 1. Descarrega a seção em uma pilha auxiliar.
                // Isso inverte a ordem, resultando em uma ordem FIFO (pacote mais antigo no topo).
                Pilha pilha_auxiliar_fifo;
                {
                    Pilha pilha_temp_lifo;
                    // Primeiro, obtemos os pacotes na ordem LIFO original
                    while (armazens[origem].temPacotesPara(destino)) {
                        pilha_temp_lifo.Empilha(new Celula(armazens[origem].recupera(destino)));
                    }
                    
                    // Imprime os eventos de remoção, avançando o relógio a cada passo
                    while(!pilha_temp_lifo.Vazia()){
                        Pacote p = pilha_temp_lifo.Desempilha();
                        
                        // O relógio da simulação avança AQUI
                        this->tempoAtual += custoRemocao;
                        
                        printf("%07d pacote %03d removido de %03d na secao %03d\n", this->tempoAtual, p.getChave(), origem, destino);
                        
                        // Empilha na auxiliar para processamento FIFO
                        pilha_auxiliar_fifo.Empilha(new Celula(p));
                    }
                }
                
                // 2. Processa transporte/rearmazenamento da pilha auxiliar (que está em ordem FIFO)
                int pacotes_transportados = 0;
                while (!pilha_auxiliar_fifo.Vazia()) {
                    // Desempilha o pacote mais antigo primeiro
                    Pacote p = pilha_auxiliar_fifo.Desempilha();

                    if (pacotes_transportados < capacidade) {
                        // Pacote vai para o transporte
                        printf("%07d pacote %03d em transito de %03d para %03d\n", this->tempoAtual, p.getChave(), origem, destino);
                        
                        Evento chegada_pacote(this->tempoAtual + latencia, 1, p.getChave(), destino);
                        this->insereEvento(chegada_pacote);
                        pacotes_transportados++;
                    } else {
                        // Pacote é rearmazenado
                        printf("%07d pacote %03d rearmazenado em %03d na secao %03d\n", this->tempoAtual, p.getChave(), origem, destino);
                        armazens[origem].armazena(p, destino);
                    }
                }

                // 3. Reagenda o próximo transporte usando o tempo de INÍCIO do evento atual
                Evento proximo_transporte(tempo_inicio_evento + intervalo, 2, origem, destino);
                this->insereEvento(proximo_transporte);
                
                // --- FIM DA LÓGICA ADAPTADA --

            } else if (this->numPacotesEntregues < this->totalPacotes) {
                // Se não há pacotes agora, mas a simulação não acabou, agenda o próximo transporte mesmo assim.
                Evento proximo_transporte(tempo_inicio_evento + intervalo, 2, origem, destino);
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