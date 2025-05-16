#include "ordenadorUniversal.hpp"
#include <iostream>
#include <cmath>
int median (int a, int b, int c) {
    if ((a <= b) && (b <= c)) return b;  // a b c
    if ((a <= c) && (c <= b)) return c;  // a c b
    if ((b <= a) && (a <= c)) return a;  // b a c
    if ((b <= c) && (c <= a)) return c;  // b c a
    if ((c <= a) && (a <= b)) return a;  // c a b
    return b;                            // c b a
  }

Ordenador::Ordenador(MeuVetor<int> vetor,int tamanho,int limiarParticao,int limiarQuebras){
        this->vetor = vetor;
        this->limiarParticao = limiarParticao;
        this->limiarQuebras = limiarQuebras;
    }

void Ordenador::ordenacao(){
    /* if numero quebras < limiarquebras insercao */
    if(tamanho>limiarParticao){
        quickSort3Ins(0,this->tamanho);
    }
    else{
        insercao(0,this->tamanho);
    }
}
/* DESCOBRIR COMO PASSAR O ATRIBUTO LIMPARTICAO CORRETAMENTE: NAO PASSAR SOMENTE O INDICE, MAS O ELEMENTO */
void Ordenador::determinaLimiarParticao(float limiarCusto){
    int minMPS = 2;
    int maxMPS = tamanho;
    int passoMPS = (maxMPS - minMPS)/5;
    int numMPS = 6,limParticao = 0,newMin = 0,newMax = 0;
    float diffCusto = 0; 
    while((diffCusto > limiarCusto) && (numMPS >= 5)){
        MeuVetor<int> custo,mpsAsspciado;
        numMPS = 0;
        for(int MPS = minMPS; MPS <=maxMPS; MPS+=passoMPS ){
            Ordenador ordenador(vetor,tamanho,MPS,limiarQuebras);
            ordenador.ordenacao(); 
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
        minMPS = mpsAsspciado[newMin];
        maxMPS = mpsAsspciado[newMax];
        passoMPS = (maxMPS - minMPS)/5;
        if(passoMPS == 0){
            passoMPS++;
        }
        diffCusto = std::fabs(custo[newMin] - custo[newMax]);
        std::cout << "nummps " << numMPS << " " << "limParticao " << limParticao << " " << "mpsdiff " << diffCusto << std::endl;
    }
    this->limiarParticao = limParticao;
}

void Ordenador::quickSort3Ins(int l,int r){
    this->estatisticas[4]++;
    int i,j;
    partition3(l,r,&i,&j);
    if(l<j){
      if (j - l <= this->limiarParticao) {
        this->insercao(l, j);
      }
      else{quickSort3Ins(l,j);}
    }
    
    if(r>i){
      if (r - i <= limiarParticao) {
        insercao(i, r);
      }
      else{quickSort3Ins(i,r);
      }
    } 
}

void Ordenador::partition3(int l, int r, int *i, int *j) {
    this->estatisticas[4]++;
    *i = l; *j = r;
    int a = this->vetor[l];
    int b = this->vetor[((l+r)/2)];
    int c = this->vetor[r];
    int p = median(a, b, c);
  
    do{
        while(p > this->vetor[*i]){
            (*i)++;this->estatisticas[2]++;
        }
        this->estatisticas[2]++;
        while(p < this->vetor[*j]){
            (*j)--;this->estatisticas[2]++;
        }
        this->estatisticas[2]++;
        if(*i<=*j){
            this->vetor.troca(*i,*j);
            this->estatisticas[3]++;
            (*i)++;
            (*j)--;
        }
    }
    while(*i<=*j);
  }

void Ordenador::insercao(int l, int r){
    this->estatisticas[4]++;
    int i,j,aux;
    for(i=l+1;i<=r;i++){
      
      aux = vetor[i];
      this->estatisticas[3]++;
  
      j = i - 1;
  
      while(j>=l && vetor[j]){
        this->estatisticas[2];
        vetor.atribuirValor(j+1,vetor[j]);
        this->estatisticas[3]++;
        j--;
      }
      this->estatisticas[2];
      vetor.atribuirValor(j+1,aux);
      this->estatisticas[3]++;
    }
    return;
}

void Ordenador::imprimeEstatisticas(){
    std::cout << "mps " << estatisticas[0] << " ";
    std::cout << "cost " << estatisticas[0] << " ";
    std::cout << "cmp " << estatisticas[0] << " ";
    std::cout << "move " << estatisticas[0] << " ";
    std::cout << "calls " << estatisticas[0] << "\n"; 
}

void Ordenador::determinaLimiarQuebras(int limiarCusto,int limiarParticao){
    int minQPS = 2;
    int maxQPS = this->tamanho;
    int passoQPS = (maxQPS - minQPS)/5;
    int numQPS = 6,limParticao = 0,newMin = 0,newMax = 0;
    float diffCusto = 0; 
    while((diffCusto > limiarCusto) && (numQPS >= 5)){
        MeuVetor<int> custo,mpsAsspciado;
        numQPS = 0;
        for(int QPS = minQPS; QPS <=maxQPS; QPS+=passoQPS ){
            Ordenador ordenador(vetor,tamanho,limiarParticao,limiarQuebras);
            ordenador.ordenacao(); 
            imprimeEstatisticas();
            custo.adicionar(ordenador.estatisticas[1]);
            mpsAsspciado.adicionar(QPS);
            numQPS++;
        }
        limParticao = custo.menorIndice();
        if(limParticao == 0 ){
            newMin = 0;
            newMax = 2;
        }
        else if(limParticao == numQPS-1){
            newMin = numQPS-3;
            newMax = numQPS-1;
        }
        else{
            newMin = limiarParticao-1;
            newMax = limiarParticao+1;
        }
        minQPS = mpsAsspciado[newMin];
        maxQPS = mpsAsspciado[newMax];
        passoQPS = (maxQPS - minQPS)/5;
        if(passoQPS == 0){
            passoQPS++;
        }
        diffCusto = std::fabs(custo[newMin] - custo[newMax]);
        std::cout << "nummps " << numQPS << " " << "limParticao " << limParticao << " " << "mpsdiff " << diffCusto << std::endl;
    }
    this->limiarParticao = limParticao;
}; /* IMPLEMENTAR */
void Ordenador::suffleVector(int numShuffle){
    int p1 = 0, p2 = 0, temp;
    for (int t = 0; t < numShuffle; t++) {
        /* Gera dois índices distintos no intervalo [0..size-1] */
        while (p1 == p2) {
            p1 = (int)(drand48() * this->tamanho);
            p2 = (int)(drand48() * this->tamanho);
        }
    /* Realiza a troca para introduzir uma quebra */
        temp = this->vetor[p1];
        this->vetor[p1] = this->vetor[p2];
        this->vetor[p2] = temp;
        p1 = p2 = 0;
    }
}