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
// Em TP2/src/escalonador.cpp, dentro do método Escalonador::inicializa

        if (proximo_evento.getTipo() == 2) { // Evento de Transporte
            int origem = proximo_evento.getOrigemTransporte();
            int destino = proximo_evento.getDestinoTransporte();
            int tempo_inicio_evento = this->tempoAtual;

            if (armazens[origem].temPacotesPara(destino)) {

                // --- INÍCIO DA LÓGICA FINAL ---

                // Esta pilha conterá os pacotes em ordem de processamento FIFO (mais antigo no topo)
                Pilha pilha_processamento_fifo;
                
                // Passo 1: Esvaziar a seção do armazém.
                // A remoção física segue a ordem LIFO. Imprimimos o evento aqui.
                while (armazens[origem].temPacotesPara(destino)) {
                    Pacote p = armazens[origem].recupera(destino);
                    
                    this->tempoAtual += custoRemocao;
                    
                    printf("%07d pacote %03d removido de %03d na secao %03d\n", this->tempoAtual, p.getChave(), origem, destino);
                    
                    // Empilha na nossa pilha de processamento.
                    // Esta única inversão já deixa os pacotes em ordem FIFO para processamento.
                    pilha_processamento_fifo.Empilha(new Celula(p));
                }

                // --- MUDANÇA IMPORTANTE AQUI ---
                // A pilha `pilha_processamento_fifo` está na ordem inversa da original,
                // o que significa que o pacote mais antigo está no topo.
                // Vamos processar DIRETAMENTE dela.

                int pacotes_transportados = 0;
                Pilha pacotes_para_rearmazenar;

                while (!pilha_processamento_fifo.Vazia()) {
                    // Desempilha() agora obtém o pacote MAIS ANTIGO primeiro.
                    Pacote p = pilha_processamento_fifo.Desempilha();

                    if (pacotes_transportados < capacidade) {
                        // Pacote selecionado para transporte (lógica FIFO)
                        printf("%07d pacote %03d em transito de %03d para %03d\n", this->tempoAtual, p.getChave(), origem, destino);
                        Evento chegada_pacote(this->tempoAtual + latencia, 1, p.getChave(), destino);
                        this->insereEvento(chegada_pacote);
                        pacotes_transportados++;
                    } else {
                        // Pacote excedente é guardado para ser rearmazenado
                        pacotes_para_rearmazenar.Empilha(new Celula(p));
                    }
                }

                // Passo 3: Rearmazenar os pacotes excedentes.
                while (!pacotes_para_rearmazenar.Vazia()) {
                    Pacote p = pacotes_para_rearmazenar.Desempilha();
                    printf("%07d pacote %03d rearmazenado em %03d na secao %03d\n", this->tempoAtual, p.getChave(), origem, destino);
                    armazens[origem].armazena(p, destino);
                }

                // Passo 4: Agendar o próximo evento de transporte.
                Evento proximo_transporte(tempo_inicio_evento + intervalo, 2, origem, destino);
                this->insereEvento(proximo_transporte);
                
                // --- FIM DA LÓGICA FINAL ---

            } else if (this->numPacotesEntregues < this->totalPacotes) {
                Evento proximo_transporte(tempo_inicio_evento + intervalo, 2, origem, destino);
                this->insereEvento(proximo_transporte);
            }
        }
        // O resto do seu código no loop while permanece igual
    
        else if (proximo_evento.getTipo() == 1) { // Evento de Chegada de Pacote
            
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
                // --- LÓGICA CORRIGIDA AQUI ---

                // 1. O pacote chegou fisicamente em 'local_chegada'.
                //    Verificamos se este local JÁ é o seu destino final.
                if (local_chegada == pacote_atual->getDestinoFinal()) {
                    pacote_atual->alteraEstado("Entregue");
                    printf("%07d pacote %03d entregue em %03d\n", this->tempoAtual, id_pacote_chegou, local_chegada);
                    this->AtualizaEstatisticas(*pacote_atual);
                } 
                // 2. Se não é o destino final, o pacote precisa ser armazenado
                //    para aguardar o próximo transporte.
                else {
                    // 2a. ANTES de avançar, pegamos qual é o próximo destino na rota.
                    int proximo_destino_na_rota = pacote_atual->getProximoDestinoNaRota();
                    
                    if (proximo_destino_na_rota != -1) {
                        // 2b. Armazenamos o pacote no armazém ATUAL (local_chegada),
                        //     na seção que leva ao PRÓXIMO destino.
                        armazens[local_chegada].armazena(*pacote_atual, proximo_destino_na_rota);
                        pacote_atual->alteraEstado("Armazenado");
                        printf("%07d pacote %03d armazenado em %03d na secao %03d\n", this->tempoAtual, id_pacote_chegou, local_chegada, proximo_destino_na_rota);
                        
                        // 2c. AGORA que já usamos a informação, avançamos o ponteiro interno da rota.
                        pacote_atual->avancarRota();
                    } else {
                        // Este é o local do seu "CHAMAR O GEMINI". Ele indica um erro lógico.
                        // Se um pacote não está no destino final, ele DEVE ter um próximo passo.
                        // Se não tem, a rota foi calculada errada ou gerenciada de forma incorreta.
                        // Com a lógica corrigida, este trecho não deve mais ser alcançado.
                        std::cerr << "ERRO LOGICO INESPERADO: Pacote " << id_pacote_chegou 
                                  << " em " << local_chegada 
                                  << " sem proximo destino definido." << std::endl;
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