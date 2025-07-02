#include "armazem.hpp"

// Construtor padrao
Armazem::Armazem() 
: secoes(nullptr), numPacotes(0), numArmazens(0), id(-1) {}

// Construtor
Armazem::Armazem(int _id, int n, int na) 
: secoes(nullptr), numPacotes(n), numArmazens(na), id(_id)
{
    secoes = new Pilha[numArmazens];
}

// Construtor de copia 
Armazem::Armazem(const Armazem& outro) 
: numPacotes(outro.numPacotes), numArmazens(outro.numArmazens), id(outro.id)
{  
    if (numArmazens > 0) {
        secoes = new Pilha[numArmazens];
        for (int i = 0; i < numArmazens; i++) {
            secoes[i] = outro.secoes[i];
        }
    } else {
        secoes = nullptr;
    }
}

Armazem& Armazem::operator=(const Armazem& outro) {
    if (this == &outro) {
        return *this;
    }

    numPacotes = outro.numPacotes; 
    numArmazens = outro.numArmazens;
    id = outro.id;

    if (secoes != nullptr) {
        delete[] secoes;
        secoes = nullptr;
    }

    if (numArmazens > 0) {
        secoes = new Pilha[numArmazens];
        for(int i = 0; i < numArmazens; i++) {
            secoes[i] = outro.secoes[i];
        }
    } else { 
        secoes = nullptr;
    }
    

    return *this;
}

// Destrutor
Armazem::~Armazem() {
    delete[] secoes;
}

// Getters e Setters
const int Armazem::getId() {
    return this->id;
}

const int Armazem::getNumPacotes() {
    return numPacotes;
}

void Armazem::setId(int _id) {
    this->id = _id;
}

void Armazem::setNumPacotes(int n) {
    numPacotes = n;
}

// Operacoes Hanoi
void Armazem::armazenaPacote(Pacote &p) {
    int secao_id = p.getRota()->GetPrimeira()->GetItem().GetChave();

    if (secao_id < 0 || secao_id >= numArmazens) {
        std::cerr << "ERRO: secao " << secao_id
        << " inexistente no armazenamento do pacote " 
        << p.getId() << "\n";
        return;
    }

    secoes[secao_id].Empilha(p.getId());
    p.setEstado(ARMAZENADO);
}

int Armazem::recuperaPacote(int secao_id) {
    if (secao_id >= numArmazens || secao_id < 0) {
        std::cerr << "ERRO: secao de armazem inexistente.\n";
        return -1;
    }
    if (!secoes[secao_id].Vazia()) {
        int pacote_id = secoes[secao_id].Desempilha().GetChave();
        return pacote_id;
    } else {
        std::cerr << "ERRO: secao vazia\n";
        return -1;
    }
}

bool Armazem::secaoVazia(int secao_id) {
    return secoes[secao_id].Vazia();
}
