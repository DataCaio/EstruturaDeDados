#include "minHeap.hpp"
#include <iostream>
Heap::Heap(){
    this->tamanho = 0;
}


Heap::Heap(int maxsize){
    tamanho = 0;
    data = new int[maxsize];
}

Heap::~Heap(){
    delete[] data;
}

void Heap::Inserir(int x){
    data[tamanho] = x;
    HeapifyPorCima(tamanho);
    tamanho++;
}

int Heap::Remover() {
    if (Vazio()) {
        return -1;
    }
    else{
        int raiz = data[0];
        data[0] = data[tamanho - 1];
        tamanho--;
        HeapifyPorBaixo(0);
        return raiz;
    }
}

bool Heap::Vazio(){
    return (tamanho == 0);
    printf("Heap Vazio!");
}

int Heap::GetAncestral(int posicao){
    return ((posicao-1)/2);
}

int Heap::GetSucessorEsq(int posicao){
    return ((posicao*2)+1);
}

int Heap::GetSucessorDir(int posicao){
    return ((posicao*2)+2);
}
void Heap::HeapifyPorCima(int posicao) {
    while (posicao > 0) {
        int pai = GetAncestral(posicao);
        if (data[posicao] < data[pai]) {
            std::swap(data[posicao], data[pai]);
            posicao = pai;
        } else {
            break;
        }
    }
}
void Heap::HeapifyPorBaixo(int posicao){
    int menor = posicao;
    while(true){
        int esq = GetSucessorEsq(posicao);
        int dir = GetSucessorDir(posicao);
        if(esq < tamanho && data[esq] < data[menor]){
            menor = esq;
        }
        
        if(dir < tamanho && data[dir] < data[menor]){
            menor = dir;
        }
        if (menor != posicao) {
            std::swap(data[posicao], data[menor]);
            posicao = menor;
        } else {
            break;
        }
        
    }
}