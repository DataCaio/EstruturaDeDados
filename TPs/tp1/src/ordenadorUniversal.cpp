#include "ordenadorUniversal.hpp"

Ordenador::Ordenador(MeuVetor<int> vetor,int tamanho,int limiarParticao
    ,int limiarQuebras){
        this->vetor = vetor;
        this->limiarParticao = limiarParticao;
        this->limiarQuebras = limiarQuebras;
    }

void Ordenador::ordenacao(){
    if(tamanho>limiarParticao){
        quickSort();
    }
    else{
        insercao();
    }
}

void Ordenador::determinaLimiarParticao(int limiarCusto){
    int minMPS = 2;
    int maxMPS = tamanho;
    int passoMPS = (maxMPS - minMPS)/5;
    int diffCusto = 0, numMPS = 0,limParticao = 0,newMin = 0,newMax = 0;
    while((diffCusto > limiarCusto)){
        MeuVetor<int> custo,mpsAsspciado;
        numMPS = 0;
        for(int MPS = minMPS; MPS <=maxMPS; MPS+=passoMPS ){
            Ordenador ordenador(vetor,tamanho,MPS,limiarQuebras);
            ordenador.ordenacao(); /* precisa colocar a anotar estatisticas */
            imprimeEstatisticas();
            custo.adicionar(ordenador.estatisticas[1]);
            mpsAsspciado.adicionar(MPS);
            numMPS++;
        }
        limParticao = custo.menorIndice();
        if(limParticao == 0 ){
            newMin = 0;
            newMax = 2;
        }
        else if(limParticao == numMPS-1){
            newMin = numMPS-3;
            newMax = numMPS-1;
        }
        else{
            newMin = limiarParticao-1;
            newMax = limiarParticao+1;
        }
        minMPS = mpsAsspciado.operator[](newMin);
        maxMPS = mpsAsspciado.operator[](newMax);
        passoMPS = (maxMPS - minMPS)/5;
        if(passoMPS == 0){
            passoMPS++;
        }
    }
    limiarParticao = limParticao;
}