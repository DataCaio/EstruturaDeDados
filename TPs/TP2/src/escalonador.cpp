#include "escalonador.hpp"
#include "iostream"

Escalonador::Escalonador() : eventos() { // Chama o construtor padrão de Heap
    this->tempoAtual = 0;
}

// O NOVO CONSTRUTOR COM A SOLUÇÃO CORRETA
Escalonador::Escalonador(int capacidade) : eventos(capacidade) { // <-- A MÁGICA ACONTECE AQUI
    // A sintaxe ': eventos(capacidade)' diz ao C++:
    // "Antes de executar o código deste construtor, por favor,
    // construa o membro 'eventos' usando o construtor de Heap que aceita um int,
    // e passe 'capacidade' para ele."

    // Agora, podemos inicializar os outros membros no corpo.
    this->tempoAtual = 0;
}

void Escalonador::inicializa(Pacote* pacotes, int numPacotes, Armazem* armazens, int numArmazens, Transporte& transporte, int latencia, int intervalo, int capacidade, int custoRemocao) {
    while (!this->eventos.Vazio()) {
        
        Evento proximo_evento = this->retiraEvento();
        this->tempoAtual = proximo_evento.getTempo();

        // Se evento é transporte
        // Substitua o bloco 'if (proximo_evento.getTipo() == 2)' por este:
        if (proximo_evento.getTipo() == 2) {
        int origem = proximo_evento.getOrigemTransporte();
        int destino = proximo_evento.getDestinoTransporte();

        // Inicia o tempo de partida com o tempo do evento atual
        int tempo_partida = this->tempoAtual;

        if (armazens[origem].temPacotesPara(destino)) {
            Pilha pilha_temp;

            // >>> A CORREÇÃO FINAL ESTÁ AQUI <<<
            // Aplica um "custo de acesso" inicial para começar o processo de remoção.
            // Isso alinha os tempos com o arquivo de saída de exemplo.
            tempo_partida += custoRemocao;

            // 1. Esvaziar a seção...
            while (armazens[origem].temPacotesPara(destino)) {
                Pacote pacote_removido = armazens[origem].recupera(destino);
                
                // O tempo do evento de remoção é o tempo atual da partida.
                printf("%07d pacote %03d removido de %03d na secao %03d\n", tempo_partida, pacote_removido.getChave(), origem, destino);
                pilha_temp.Empilha(new Celula(pacote_removido));

                // Se ainda houver pacotes, o tempo para a *próxima* remoção aumenta.
                if (armazens[origem].temPacotesPara(destino)) {
                    tempo_partida += custoRemocao;
                }
            }

            // 2. Transportar pacotes...
            int pacotes_transportados = 0;
            while (!pilha_temp.Vazia() && pacotes_transportados < capacidade) {
                Pacote pacote_a_transportar = pilha_temp.Desempilha();
                pacotes_transportados++;

                printf("%07d pacote %03d em transito de %03d para %03d\n", tempo_partida, pacote_a_transportar.getChave(), origem, destino);
                
                int tempo_chegada_destino = tempo_partida + latencia;
                Evento evento_chegada(tempo_chegada_destino, 1, pacote_a_transportar.getChave(), destino);
                this->insereEvento(evento_chegada);
            }

            // 3. Rearmazenar...
            while (!pilha_temp.Vazia()) {
                Pacote pacote_a_rearmazenar = pilha_temp.Desempilha();
                armazens[origem].armazena(pacote_a_rearmazenar, destino);
                printf("%07d pacote %03d rearmazenado em %03d na secao %03d\n", tempo_partida, pacote_a_rearmazenar.getChave(), origem, destino);
            }
        }
        
        // Agenda o próximo evento de transporte relativo ao tempo de TÉRMINO desta operação.
        int tempo_proximo_transporte = tempo_partida + intervalo;
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
    
    // Calcula o tempo total de vida do pacote (tempo atual - tempo de postagem)
    int tempoDeVida = this->tempoAtual - pacote_entregue.getTempoChegada();
    this->tempoTotalEmSistema += tempoDeVida;
}
void Escalonador::MostraEstatisticas() {
    std::cout << "--- ESTATISTICAS FINAIS ---" << std::endl;
    std::cout << "Tempo total da simulacao: " << this->tempoAtual << std::endl;
    std::cout << "Numero de pacotes entregues: " << this->numPacotesEntregues << std::endl;
    
    if (numPacotesEntregues > 0) {
        double tempoMedio = tempoTotalEmSistema / numPacotesEntregues;
        std::cout << "Tempo medio de entrega por pacote: " << tempoMedio << std::endl;
    } else {
        std::cout << "Nenhum pacote foi entregue." << std::endl;
    }
    std::cout << "---------------------------" << std::endl;
}