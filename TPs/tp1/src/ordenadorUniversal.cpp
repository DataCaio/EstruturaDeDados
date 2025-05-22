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

void Ordenador::resetaEstatisticas(){
    for (int i = 0; i < 5; i++){
        estatisticas[0] = 0;
    };
    
};

Ordenador::Ordenador(MeuVetor<int> vetor,int tamanho,int limiarParticao,int limiarQuebras, int constantes[3]){
        this->vetor = vetor;
        this->limiarParticao = limiarParticao;
        this->limiarQuebras = limiarQuebras;
        for(int i=0; i<3 ;i++){
            this->constantes[i] = constantes[i];
        }
    }

void Ordenador::ordenacao(){
    /* if numero quebras < limiarquebras insercao */
    if(tamanho>limiarParticao){
        quickSort3Ins(0,this->tamanho-1);
    }
    else{
        insercao(0,this->tamanho-1);
    }
}

int Ordenador::determinaLimiarParticao(float limiarCusto){
    int idx = 0;
    int minMPS = 2;
    int maxMPS = tamanho;
    int passoMPS = (maxMPS - minMPS)/5;
    int numMPS = 6,limParticao = 0,newMin = 0,newMax = 0,melhorLimiar=0;
    float diffCusto = limiarCusto + 1; 
    while((diffCusto > limiarCusto) && (numMPS >= 5)){
        std::cout << std::endl << "iter " << idx++ << std::endl;
        MeuVetor<int> custo,mpsAsspciado;
        numMPS = 0;
        for(int MPS = minMPS; MPS <=maxMPS; MPS+=passoMPS ){
            Ordenador ordenador(this->vetor,this->tamanho,MPS,this->limiarQuebras,this->constantes);
            ordenador.ordenacao();
            ordenador.calculaCusto();
            std::cout << "mps ";
            ordenador.imprimeEstatisticas();
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
        melhorLimiar = mpsAsspciado[limiarParticao];
        std::cout << "nummps " << numMPS << " " << "limParticao " << melhorLimiar << " " << "mpsdiff " << diffCusto << std::endl;
    }
    this->limiarParticao = melhorLimiar;
    return melhorLimiar;
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
    std::cout << estatisticas[0] << " ";
    std::cout << "cost " << estatisticas[1] << " ";
    std::cout << "cmp " << estatisticas[2] << " ";
    std::cout << "move " << estatisticas[3] << " ";
    std::cout << "calls " << estatisticas[4] << "\n"; 
}

int Ordenador::determinaLimiarQuebras(int limiarCusto,int limiarParticao){
    int idx = 0;

    int minQPS = 1;
    int maxQPS = this->tamanho/2;

    int passoQPS = (maxQPS - minQPS)/5;

    int numQPS = 6,limQuebras = 0,newMin = 0,newMax = 0;

    float diffCusto = limiarCusto + 1; 

    int melhorQuebra;
    this->quickSort3Ins(0,this->tamanho-1);
    this->resetaEstatisticas();

    while((diffCusto > limiarCusto) && (numQPS >= 5)){
        std::cout << std::endl << "iter " << idx++ << std::endl;
        MeuVetor<int> custoIN,custoQS,qpsAssociado;
        numQPS = 0;
        for(int QPS = minQPS; QPS <=maxQPS; QPS+=passoQPS ){
            Ordenador ordenador(this->vetor,this->tamanho,this->limiarParticao,QPS,this->constantes);
            
            ordenador.suffleVector(QPS);
            ordenador.quickSort3Ins(0,this->tamanho-1);
            ordenador.calculaCusto();
            std::cout << "qs lq";
            ordenador.imprimeEstatisticas();
            custoQS.adicionar(ordenador.estatisticas[1]);

            ordenador.resetaEstatisticas();
            ordenador.suffleVector(QPS);
            ordenador.insercao(0,this->tamanho-1);
            ordenador.calculaCusto();
            std::cout << "in lq";
            ordenador.imprimeEstatisticas();
            custoIN.adicionar(ordenador.estatisticas[1]);
            qpsAssociado.adicionar(QPS);
            numQPS++;
        }
        
        limiarQuebras = 0;
        for(int i =0;i<numQPS;i++){
            if(fabs(custoQS[i] - custoIN[i]) < fabs(custoQS[limQuebras] - custoIN[limQuebras])){
                limQuebras = i;
            }
        }
        if(limQuebras == 0 ){
            newMin = 0;
            newMax = 2;
        }
        else if(limQuebras == numQPS-1){
            newMin = numQPS-3;
            newMax = numQPS-1;
        }
        else{
            newMin = limiarParticao-1;
            newMax = limiarParticao+1;
        }
        minQPS = qpsAssociado[newMin];
        maxQPS = qpsAssociado[newMax];
        passoQPS = (maxQPS - minQPS)/5;
        if(passoQPS == 0){
            passoQPS++;
        }
        melhorQuebra = qpsAssociado[limQuebras];
        diffCusto = std::fabs(custoIN[newMin] - custoIN[newMax]);
        std::cout << "numlq " << numQPS << " " << "limQuebras " << melhorQuebra << " " << "lqdiff " << diffCusto << std::endl;
    }
    this->limiarQuebras = melhorQuebra;
    return melhorQuebra;
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

int Ordenador::calculaQuebras(){
    int quebras = 0;
    for (int i = 1; i < tamanho; i++) {
        if (vetor[i] < vetor[i - 1]) {
            quebras++;
        }
    }
    return quebras;
}

void Ordenador::calculaCusto(){
    estatisticas[1] = constantes[0] * estatisticas[2] + constantes[1] * estatisticas[3] + constantes[2] * estatisticas[4];
}