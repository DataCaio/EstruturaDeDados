#include "simulador.hpp"

#define NUM_MAX_EVENTOS 10000

// Construtor padrao
// Inicializacao de algumas variaveis de estado aqui
Simulador::Simulador() : 
armazens(nullptr), 
pacotes(nullptr), 
Topologia(nullptr), 
Escalonador(nullptr), 
matrizAdjAux(nullptr),
numPacotesEntregues(0),
relogio(0)
{
    Escalonador = new Heap(NUM_MAX_EVENTOS);
}

// Destrutor
Simulador::~Simulador() {
    delete Topologia;
    Topologia = nullptr;
    delete Escalonador;
    Escalonador = nullptr;

    delete[] armazens;
    armazens = nullptr;
    delete[] pacotes;
    pacotes = nullptr;

    if (matrizAdjAux != nullptr) {
        for (int i = 0; i < numArmazens; i++) {
            delete[] matrizAdjAux[i]; // Libera cada linha
        }
        delete[] matrizAdjAux; // Libera o vetor de ponteiros
        matrizAdjAux = nullptr;
    }
}

void Simulador::leArquivo(std::ifstream& in) {
    in >> capacidadeTransporte
    >> latenciaTransporte
    >> intervaloTransporte
    >> custoRemocao;
    in >> numArmazens;
    
    // Inicializa topologia e matriz de adjacencia auxiliar
    matrizAdjAux = new bool*[numArmazens];
    for (int i = 0; i < numArmazens; i++) {
        matrizAdjAux[i] = new bool[numArmazens];
    }
    for (int i = 0; i < numArmazens; i++) {
        for (int j = 0; j < numArmazens; j++) {
            in >> matrizAdjAux[i][j];
        }
    }
    Topologia = new Grafo(numArmazens);
    Topologia->leGrafo(in, matrizAdjAux);
    
    in >> numPacotes;

    // Inicializa armazens
    armazens = new Armazem[numArmazens];
    for (int i = 0; i < numArmazens; i++) {
        Armazem armazem(i, numPacotes, numArmazens);
        armazens[i] = armazem;
    }

    // Inicializa pacotes
    pacotes = new Pacote[numPacotes];
    for (int i = 0; i < numPacotes; i++) {
        std::string temp;

        int origem_id, destino_id;
        int tempoChegada, id_pacote = i;
        
        // le entrada ignorando dados que nao sao cruciais
        in >> tempoChegada >> temp >> 
        temp >> temp >> // ignora id de pacote fornecido na entrada
        origem_id >> temp >> destino_id;

        Pacote pacote(tempoChegada, id_pacote, 
            origem_id, destino_id);
        
        Lista * rota = Topologia->bfs(armazens[pacote.getOrigem()], 
                        armazens[pacote.getDestino()]);
        pacote.setRota(rota);

        pacotes[i] = pacote;
    }
}

void Simulador::simulacao() {
    if (numPacotes == 0) {
        std::cerr << "ERRO: nao ha pacotes para a simulacao\n";
        return;
    }

    // Escalona chegada de pacotes nos armazens de postagem
    for (int i = 0; i < numPacotes; i++) {
        int origem = pacotes[i].getOrigem();
        int destino = pacotes[i].getDestino();

        Evento evento(pacotes[i].getTempoChegada(),
        1, origem, destino, pacotes[i].getId());
        Escalonador->InsereEvento(evento);
    }

    // Inicializa variaveis de estado
    numPacotesEntregues = 0;

    // Obtem tempo em que chegou o primeiro pacote
    Evento aux = Escalonador->RetiraProximoEvento();
    Escalonador->InsereEvento(aux);
    
    // Coloca relogio no estado "inicial"
    setRelogio(aux.tempo);

    // Escalona eventos de transporte inicial,
    // para cada ligacao entre dois armazens
    for (int i = 0; i < numArmazens; i++) {
        for (int j = 0; j < numArmazens; j++) {
            if (matrizAdjAux[i][j]) {
                int tempo = relogio + intervaloTransporte;

                // Escalona evento de transporte simples, sem pacotes
                Evento evento(tempo, 2, i, j, -1);
                Escalonador->InsereEvento(evento);
            }
        }
    }

    while (!Escalonador->Vazio()) {
        // Condicao de termino adicional 
        if (numPacotesEntregues == numPacotes) {
            break;
        }

        Evento proximo_evento = Escalonador->RetiraProximoEvento();
        relogio = proximo_evento.tempo;

        switch(proximo_evento.tipo) {
            case 1: { // armazenamento ou entrega de pacote
                int origem_atual_pacote = proximo_evento.origem_id;
                Pacote& pacote = pacotes[proximo_evento.idPacote];

                pacote.getRota()->RemoveInicio(); // Avanca na rota
                int secao_id = -1;
                if (!pacote.getRota()->Vazia()) {
                    secao_id = pacote.getRota()->GetPrimeira()->GetItem().GetChave();
                }

                if (secao_id == -1) { // Lista da rota vazia, chegou no destino  
                    numPacotesEntregues++;
                    pacote.setEstado(ENTREGUE);
                    std::cout << std::setfill('0') << std::setw(7) << relogio 
                              << " pacote " << std::setfill('0') << std::setw(3) << pacote.getId()
                              << " entregue em " << std::setfill('0') << std::setw(3) << pacote.getDestino()
                              << std::endl;
                } else { // Pacote armazenado em armazem intermediario
                    armazens[origem_atual_pacote].armazenaPacote(pacote);
                    
                    if (pacote.getOrigem() == origem_atual_pacote) {
                        pacote.setEstado(CHEGOU_ARMAZEM);
                    } else {
                        pacote.setEstado(ARMAZENADO);
                    }
                    
                    std::cout << std::setfill('0') << std::setw(7) << relogio 
                              << " pacote " << std::setfill('0') << std::setw(3) << pacote.getId()
                              << " armazenado em " << std::setfill('0') << std::setw(3) << origem_atual_pacote
                              << " na secao " << std::setfill('0') << std::setw(3) << secao_id 
                              << std::endl;
                }
                break;
            }
            case 2: { // transporte de pacotes
                Pilha auxiliar; 

                int origem_transporte = proximo_evento.origem_id;
                int destino_transporte = proximo_evento.destino_id;

                int relogio_aux = relogio;

                // Remove pacotes da secao correspondente
                while (!armazens[origem_transporte].secaoVazia(destino_transporte)) {
                    int pacote_id = armazens[origem_transporte].recuperaPacote(destino_transporte);
                    AvancaRelogio(custoRemocao);

                    // Armazenamento Hanoi: 
                    // empilha pacotes removidos na pilha auxiliar
                    auxiliar.Empilha(pacote_id);
                    pacotes[pacote_id].setEstado(REMOVIDO);
                    std::cout << std::setfill('0') << std::setw(7) << relogio 
                              << " pacote " << std::setfill('0') << std::setw(3) << pacote_id
                              << " removido de " << std::setfill('0') << std::setw(3) << origem_transporte
                              << " na secao " << std::setfill('0') << std::setw(3) << destino_transporte
                              << std::endl;
                }

                // Envia pacotes para transporte
                int pacotes_em_transito_count = 0;
                while (!auxiliar.Vazia()) {
                    int pacote_id = auxiliar.Desempilha().GetChave();
                    Pacote& p = pacotes[pacote_id];

                    if (pacotes_em_transito_count < capacidadeTransporte) {
                        // Pacote sai para transporte
                        std::cout << std::setfill('0') << std::setw(7) << relogio 
                                  << " pacote " << std::setfill('0') << std::setw(3) << pacote_id
                                  << " em transito de " << std::setfill('0') << std::setw(3) << origem_transporte
                                  << " para " << std::setfill('0') << std::setw(3) << destino_transporte
                                  << std::endl;

                        p.setEstado(EM_TRANSPORTE);

                        // Escalona chegada no prox armazem
                        Evento evento(relogio + latenciaTransporte, 1, // Tipo 1: pacote
                        destino_transporte, p.getDestino(), pacote_id);
                        
                        Escalonador->InsereEvento(evento);
                        
                        pacotes_em_transito_count++;
                    } 

                    else { // Rearmazena imediatamente quaisquer pacotes que tenham sobrado
                        armazens[origem_transporte].armazenaPacote(p);
                        p.setEstado(ARMAZENADO);

                        std::cout << std::setfill('0') << std::setw(7) << relogio 
                                  << " pacote " << std::setfill('0') << std::setw(3) << pacote_id
                                  << " rearmazenado em " << std::setfill('0') << std::setw(3) << proximo_evento.origem_id
                                  << " na secao " << std::setfill('0') << std::setw(3) << proximo_evento.destino_id 
                                  << std::endl;
                    }
                }

                // Escalona prox evento de transporte para pacotes nessa secao
                int tempo_prox_transporte = relogio_aux + intervaloTransporte;        
                Evento evento(tempo_prox_transporte, 2, // Tipo 2: transporte
                              origem_transporte, destino_transporte, -1);
                Escalonador->InsereEvento(evento);

                break;
            }
        }
    }
}

void Simulador::AvancaRelogio(int tempo) {
    relogio += tempo;
}

void Simulador::setRelogio(int tempo) {
    relogio = tempo;
}
