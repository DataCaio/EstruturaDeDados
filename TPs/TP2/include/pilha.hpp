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

        Pilha(const Pilha& outra) {
            this->topo = nullptr;
            this->tamanho = 0;
            this->vazia = true;

            if (outra.topo == nullptr) {
                return;
            }

            this->topo = new Celula(outra.topo->pacote);
            this->tamanho = 1;
            this->vazia = false;

            Celula* ultimo_novo = this->topo;
            Celula* atual_outro = outra.topo->prox;

            while (atual_outro != nullptr) {
                ultimo_novo->prox = new Celula(atual_outro->pacote);
                ultimo_novo = ultimo_novo->prox;
                this->tamanho++;
                atual_outro = atual_outro->prox;
            }
        }

        // Destrutor
        virtual ~Pilha(){
            this->limpa();
        }

        Pilha& operator=(const Pilha& outra) {
            if (this == &outra) {
                return *this;
            }

            this->limpa();

            if (outra.topo == nullptr) {
                this->topo = nullptr;
                this->tamanho = 0;
                this->vazia = true;
                return *this;
            }
            
            this->topo = new Celula(outra.topo->pacote);
            this->tamanho = 1;
            this->vazia = false;

            Celula* ultimo_novo = this->topo;
            Celula* atual_outro = outra.topo->prox;

            while (atual_outro != nullptr) {
                ultimo_novo->prox = new Celula(atual_outro->pacote);
                ultimo_novo = ultimo_novo->prox;
                this->tamanho++;
                atual_outro = atual_outro->prox;
            }

            return *this;
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

        bool Vazia() const {
            return this->vazia;
        }
};

#endif