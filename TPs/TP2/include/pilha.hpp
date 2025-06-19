#ifndef PILHA_H
#define PILHA_H

#include "celula.hpp"
#include "pacote.hpp"

class Pilha{
    private:
        Celula* topo;
        int tamanho;
        bool vazia;
    public:
        // Construtor
        Pilha(){
            this->topo = nullptr;
            this->tamanho = 0;
            this->vazia = true;
        }

        // Destrutor
        virtual ~Pilha(){
            this->limpa();
        }

        void Empilha(Celula *celula){
            celula->prox = this->topo;
            this->topo = celula;
            this->tamanho++;
            this->vazia = false;
        }

        Pacote Desempilha(){
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

        void limpa(){
            while (!this->vazia){
                Desempilha();
            }
        }

        bool Vazia() {
            return this->vazia;
        }
};

#endif