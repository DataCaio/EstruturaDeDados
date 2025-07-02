#include "escalonador.hpp"
#include "iostream"
#include "iomanip"

Escalonador::Escalonador() : eventos() { // Chama o construtor padrão de Heap
    this->tempoAtual = 0;
    this->numPacotesEntregues = 0;
    this->totalPacotes = 0;
    this->tempoTotalEmSistema = 0.0;
    this->tempoFinalSimulacao = 0;
}

Escalonador::Escalonador(int capacidade) : eventos(capacidade) { 
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

        if (proximo_evento.getTipo() == TRANSPORTE) { // Evento de Transporte
            int origem = proximo_evento.getOrigemTransporte();
            int destino = proximo_evento.getDestinoTransporte();
            int tempo_inicio_evento = this->tempoAtual;

            if (armazens[origem].temPacotesPara(destino)) {
                Pilha pilha_processamento_fifo;
                while (armazens[origem].temPacotesPara(destino)) {
                    Pacote p = armazens[origem].recupera(destino);
                    
                    this->tempoAtual += custoRemocao;
                    
                    std::cout << std::setfill('0') << std::setw(7) << this->tempoAtual
                    << " pacote " << std::setw(3) << p.getChave()
                    << " removido de " << std::setw(3) << origem
                    << " na secao " << std::setw(3) << destino << std::endl;

                    
                    // Empilha na pilha de processamento.
                    pilha_processamento_fifo.Empilha(new Celula(p));
                }

                int pacotes_transportados = 0;
                Pilha pacotes_para_rearmazenar;

                while (!pilha_processamento_fifo.Vazia()) {
                    Pacote p = pilha_processamento_fifo.Desempilha();

                    if (pacotes_transportados < capacidade) {
                        std::cout << std::setfill('0') << std::setw(7) << this->tempoAtual
                    << " pacote " << std::setw(3) << p.getChave()
                    << " em transito de " << std::setw(3) << origem
                    << " para " << std::setw(3) << destino << std::endl;
                        Evento PACOTE(this->tempoAtual + latencia, p.getChave(), destino);
                        this->insereEvento(PACOTE);
                        pacotes_transportados++;
                    } else {
                        pacotes_para_rearmazenar.Empilha(new Celula(p));
                    }
                }

                Pilha pilha_rearmazenamento_correta;
                while (!pacotes_para_rearmazenar.Vazia()) {
                    pilha_rearmazenamento_correta.Empilha(new Celula(pacotes_para_rearmazenar.Desempilha()));
                }
                while (!pilha_rearmazenamento_correta.Vazia()) {
                    Pacote p = pilha_rearmazenamento_correta.Desempilha();
                    std::cout << std::setfill('0') << std::setw(7) << this->tempoAtual
                    << " pacote " << std::setw(3) << p.getChave()
                    << " rearmazenado em " << std::setw(3) << origem
                    << " na secao " << std::setw(3) << destino << std::endl;
                    armazens[origem].armazena(p, destino);
                }
                Evento proximo_transporte(tempo_inicio_evento + intervalo, origem, destino, true);
                this->insereEvento(proximo_transporte);
            } else if (this->numPacotesEntregues < this->totalPacotes) {
                Evento proximo_transporte(tempo_inicio_evento + intervalo, origem, destino, true);
                this->insereEvento(proximo_transporte);
            }
        }
    
        if (proximo_evento.getTipo() == PACOTE) {
            
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
                // 1. O pacote chegou fisicamente em 'local_chegada'.
                //    Verificamos se este local JÁ é o seu destino final.
                if (local_chegada == pacote_atual->getDestinoFinal()) {
                    pacote_atual->alteraEstado("Entregue");
                    std::cout << std::setfill('0') << std::setw(7) << this->tempoAtual
                    << " pacote " << std::setw(3) << id_pacote_chegou
                    << " entregue em " << std::setw(3) << local_chegada << std::endl;
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
                        std::cout << std::setfill('0') << std::setw(7) << this->tempoAtual
                        << " pacote " << std::setw(3) << id_pacote_chegou
                        << " armazenado em " << std::setw(3) << local_chegada
                        << " na secao " << std::setw(3) << proximo_destino_na_rota << std::endl;
                                        
                        // 2c. AGORA que já usamos a informação, avançamos o ponteiro interno da rota.
                        pacote_atual->avancarRota();
                    } else {
                        std::cerr << "ERRO LOGICO INESPERADO: Pacote " << id_pacote_chegou 
                                  << " em " << local_chegada 
                                  << " sem proximo destino definido." << std::endl;
                    }
                }
            }
        }
    }
}

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