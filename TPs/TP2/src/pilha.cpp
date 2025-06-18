#include "pilha.hpp"

Pilha::Pilha(){
    this->topo = nullptr;
    this->tamanho = 0;
    this->vazia = true;
}

Pilha::~Pilha(){
    this->limpa();
}

void Pilha::Empilha(Celula* celula){
    celula->prox = this->topo;
    this->topo = celula;
    this->tamanho++;
    this->vazia = false;
}

Pacote Pilha::Desempilha(){
    if(this->vazia || this->topo == nullptr){
        return Pacote();
    }
    Celula* velhoTopo = this->topo;
    this->topo = this->topo->prox;
    Pacote pacote = velhoTopo->pacote;
    delete velhoTopo;
    this->tamanho--;
    if(this->tamanho == 0){
        this->vazia = true;
    }
    return pacote;
}

void Pilha::limpa(){
    while (!this->vazia){
        Desempilha();
    }
}

bool Pilha::Vazia() {
    return  this->vazia;
}
