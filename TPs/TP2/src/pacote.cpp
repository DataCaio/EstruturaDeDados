#include "pacote.hpp"
Pacote::Pacote(){
    this->chave = -1;
    this->posicao_atual_na_rota = nullptr;
}
// Implementação do Destrutor
Pacote::~Pacote() {
    // O corpo do destrutor está vazio de propósito.
}
Pacote::Pacote(const Pacote& outro) :
    chave(outro.chave),
    estado(outro.estado),
    origem(outro.origem),
    destino(outro.destino),
    tempoChegada(outro.tempoChegada),
    rota(outro.rota), // << Chama o construtor de cópia de ListaEncadeada
    tempoArmazenado(outro.tempoArmazenado),
    tempoTransportado(outro.tempoTransportado)
{
    // A parte mais difícil é corrigir o ponteiro posicao_atual_na_rota.
    // Ele precisa apontar para o nó correspondente na *nova* lista.
    if (outro.posicao_atual_na_rota == nullptr) {
        this->posicao_atual_na_rota = nullptr;
    } else {
        // Encontra a posição do ponteiro na lista antiga
        No* no_antigo = outro.rota.getCabeca();
        int pos = 0;
        while(no_antigo != nullptr && no_antigo != outro.posicao_atual_na_rota) {
            no_antigo = no_antigo->proximo;
            pos++;
        }

        // Encontra o nó correspondente na nova lista
        No* no_novo = this->rota.getCabeca();
        for(int i = 0; i < pos && no_novo != nullptr; ++i) {
            no_novo = no_novo->proximo;
        }
        this->posicao_atual_na_rota = no_novo;
    }
}

Pacote::Pacote(int tempoChegada, int chave, int origem, int destino){
    this->tempoChegada = tempoChegada;
    this->chave = chave;
    this->origem = origem;
    this->destino = destino;
}

void Pacote::avancarRota() {
    if (posicao_atual_na_rota != nullptr) {
        posicao_atual_na_rota = posicao_atual_na_rota->proximo;
    }
}

void Pacote::calcularMinhaRota(Grafo& grafo) {
    // A lógica da BFS agora vive aqui dentro.
    // Ela usa os membros 'this->origem' e 'this->destino'.
    
    int num_vertices = grafo.getNumVertices(); // Idealmente: grafo.getNumVertices();
    int predecessor[num_vertices];
    bool visitados[num_vertices];
    
    for(int i = 0; i < num_vertices; ++i) {
        predecessor[i] = -1;
        visitados[i] = false;
    }

    Fila fila;
    visitados[this->origem] = true;
    fila.enfileirar(this->origem);

    while (!fila.vazia()) {
        int u = fila.desenfileirar();
        if (u == this->destino) break;

        for (int v = 0; v < num_vertices; ++v) {
            if (grafo[u][v] == 1 && !visitados[v]) {
                visitados[v] = true;
                predecessor[v] = u;
                fila.enfileirar(v);
            }
        }
    }

    // Agora, em vez de retornar uma lista, ele preenche a própria rota
    // Usando o membro 'this->rota'.
    int atual = this->destino;
    while (atual != -1) {
        this->rota.insereNoInicio(atual);
        atual = predecessor[atual];
    }
    
    // IMPORTANTE: Inicializa a posição atual para o início da rota recém-calculada
    this->posicao_atual_na_rota = this->rota.getCabeca();
}

No* Pacote::getPosicaoAtual() const {
    return posicao_atual_na_rota;
}

int Pacote::getChave() const {
    return chave;
}

void Pacote::setChave(int ch){
    this->chave = ch;
}

int Pacote::getDestinoFinal() const {
    return destino;
}

// Este método é CRUCIAL para a lógica de armazenamento.
// Ele diz para qual seção do armazém o pacote deve ir.
int Pacote::getProximoDestinoNaRota() const {
    // Verifica se estamos em um ponto válido da rota E se existe um próximo nó.
    if (posicao_atual_na_rota != nullptr && posicao_atual_na_rota->proximo != nullptr) {
        // Retorna o ID do armazém do próximo nó.
        return posicao_atual_na_rota->proximo->id_armazem;
    }
    // Se não há próximo destino, retorna um valor inválido.
    return -1; 
}

void Pacote::alteraEstado(std::string novoEstado) {
    this->estado = novoEstado;
}

std::string Pacote::getEstado() const {
    return estado;
}