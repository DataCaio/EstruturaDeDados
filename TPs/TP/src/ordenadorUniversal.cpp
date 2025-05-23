#include "ordenadorUniversal.hpp"
#include <iostream>
#include <cmath>
#include <iomanip>
int median (int a, int b, int c) {
    if ((a <= b) && (b <= c)) return b;  // a b c
    if ((a <= c) && (c <= b)) return c;  // a c b
    if ((b <= a) && (a <= c)) return a;  // b a c
    if ((b <= c) && (c <= a)) return c;  // b c a
    if ((c <= a) && (a <= b)) return a;  // c a b
    return b;                            // c b a
  }

void Ordenador::resetaEstatisticas(){
    for (int i = 0; i < 4; i++){
        estatisticas[i] = 0;
    };
    
};

Ordenador::Ordenador(const MeuVetor<int>& vetor,int tamanho,int limiarParticao,int limiarQuebras, double constantes[3])
    : vetor(vetor)
{
        this->tamanho = tamanho;
        this->limiarParticao = limiarParticao;
        this->limiarQuebras = limiarQuebras;
        for(int i=0; i<3 ;i++){
            this->constantes[i] = constantes[i];
        }
        this->seed = 0;
    }

void Ordenador::ordenacao(){
    int numeroQuberas = this->calculaQuebras();

    if(numeroQuberas < limiarQuebras){
        this->insercao(0,this->tamanho-1);
    }
    else if(this->tamanho>limiarParticao){
        this->quickSort3Ins(0,this->tamanho-1);
    }
    else{
        this->insercao(0,this->tamanho-1);
    }
}

int Ordenador::determinaLimiarParticao(double limiarCusto){
    int idx = 0;
    int minMPS = 2;
    int maxMPS = tamanho;
    int passoMPS = (maxMPS - minMPS)/5;
    if (passoMPS == 0) passoMPS=1;

    int numMPS = 6,limParticao = 0,newMin = 0,newMax = 0,melhorLimiar=0;
    float diffCusto = limiarCusto + 1; 
    while((diffCusto > limiarCusto) && (numMPS >= 5)){
        std::cout << std::endl << "iter " << idx++ << std::endl;
        MeuVetor<int> mpsAsspciado;
        MeuVetor<double> custo;
        numMPS = 0;
        MeuVetor<int> vetorOriginal = this->vetor;
        for(int MPS = minMPS; MPS <=maxMPS; MPS+=passoMPS ){
            MeuVetor<int> copiaVetor = vetorOriginal;
            Ordenador ordenador(copiaVetor, this->tamanho, MPS, 0, this->constantes);
            ordenador.resetaEstatisticas();
            ordenador.estatisticas[0] = MPS;

            ordenador.ordenacao();
            ordenador.calculaCusto();
            std::cout << "mps ";
            ordenador.estatisticas[0] = MPS;
            ordenador.imprimeEstatisticas();
            custo.adicionar(ordenador.custo);
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
            newMin = limParticao-1;
            newMax = limParticao+1;
        }
        minMPS = mpsAsspciado[newMin];
        maxMPS = mpsAsspciado[newMax];
        passoMPS = (maxMPS - minMPS)/5;
        if(passoMPS == 0){
            passoMPS++;
        }

        diffCusto = (float) std::fabs(custo[newMin] - custo[newMax]);
        melhorLimiar = mpsAsspciado[limParticao];

        std::cout << "nummps " << numMPS << " " << "limParticao " << melhorLimiar << " " << "mpsdiff " << std::fixed << std::setprecision(6) 
        << diffCusto << std::endl<< std::fixed << std::setprecision(9);
    }
    this->limiarParticao = melhorLimiar;
    return melhorLimiar;
}

void Ordenador::quickSort3Ins(int l,int r){
    this->estatisticas[3]++;
    int i,j;
    partition3(l,r,&i,&j);
    if(l<j){
      if (j - l < this->limiarParticao) {
        this->insercao(l, j);
      }
      else{quickSort3Ins(l,j);}
    }
    
    if(r>i){
      if (r - i < limiarParticao) {
        insercao(i, r);
      }
      else{quickSort3Ins(i,r);
      }
    } 
}

void Ordenador::partition3(int l, int r, int *i, int *j) {
    this->estatisticas[3]++;
    *i = l; *j = r;
    int a = this->vetor[l];
    int b = this->vetor[((l+r)/2)];
    int c = this->vetor[r];
    int p = median(a, b, c);
  
    do{
        while(p > this->vetor[*i]){
            (*i)++;
            this->estatisticas[1]++;
        }
        this->estatisticas[1]++;
        while(p < this->vetor[*j]){
            (*j)--;
            this->estatisticas[1]++;
        }

        this->estatisticas[1]++;
        
        if(*i<=*j){
            this->estatisticas[2] += 3;
            this->vetor.troca(*i,*j);
            (*i)++;
            (*j)--;
        }
    }
    while(*i<=*j);
  }

void Ordenador::insercao(int l, int r){
    this->estatisticas[3]++;
    int i,j,aux;
    for( i = l+1; i <= r; i++){
      
      aux = vetor[i];
      this->estatisticas[2]++;
  
      j = i - 1;
  
      while(j>=l && vetor[j] > aux ){
        this->estatisticas[1]++;
        vetor.atribuirValor(j+1,vetor[j]);
        this->estatisticas[2]++;
        j--;
      }
      this->estatisticas[1]++;
      vetor.atribuirValor(j+1,aux);
      this->estatisticas[2]++;
    }
}

void Ordenador::imprimeEstatisticas(){
    std::cout << estatisticas[0] << " ";
    std::cout << "cost " << this->custo << " ";
    std::cout << "cmp " << estatisticas[1] << " ";
    std::cout << "move " << estatisticas[2] << " ";
    std::cout << "calls " << estatisticas[3] << "\n"; 
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
        MeuVetor<int>qpsAssociado;
        MeuVetor<double> custoIN,custoQS;
        numQPS = 0;
        MeuVetor<int> vetorOriginal = this->vetor;
        for(int QPS = minQPS; QPS <=maxQPS; QPS+=passoQPS ){
            MeuVetor<int> copiaVetor = vetorOriginal;
            Ordenador ordenador(copiaVetor,this->tamanho,limiarParticao,QPS,this->constantes);
            ordenador.estatisticas[0] = QPS;
            ordenador.definirSeed(this->seed);
            ordenador.suffleVector(QPS);
            ordenador.quickSort3Ins(0,this->tamanho-1);
            ordenador.calculaCusto();
            std::cout << "qs lq ";
            ordenador.imprimeEstatisticas();
            custoQS.adicionar(ordenador.custo);

            ordenador.resetaEstatisticas();

            ordenador.estatisticas[0] = QPS;
            ordenador.suffleVector(QPS);
            ordenador.insercao(0,this->tamanho-1);
            ordenador.calculaCusto();
            std::cout << "in lq ";
            ordenador.imprimeEstatisticas();
            custoIN.adicionar(ordenador.custo);
            qpsAssociado.adicionar(QPS);
            numQPS++;
        }
        
        limQuebras = 0;
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
            newMin = limQuebras-1;
            newMax = limQuebras+1;
        }
        minQPS = qpsAssociado[newMin];
        maxQPS = qpsAssociado[newMax];
        passoQPS = (maxQPS - minQPS)/5;
        if(passoQPS == 0){
            passoQPS++;
        }
        melhorQuebra = qpsAssociado[limQuebras];
        diffCusto =(float) std::fabs(custoIN[newMin] - custoIN[newMax]);
        std::cout << "numlq " << numQPS << " " << "limQuebras " << melhorQuebra << " " << "lqdiff "   << std::fixed << std::setprecision(6) 
        << diffCusto << std::endl  << std::fixed << std::setprecision(9);
    }
    this->limiarQuebras = melhorQuebra;
    return melhorQuebra;
}; /* IMPLEMENTAR */
void Ordenador::suffleVector(int numShuffle){
    srand48(this->seed);

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
    this->custo = (constantes[0] * estatisticas[1]) + (constantes[1] * estatisticas[2]) + (constantes[2] * estatisticas[3]);
}

void Ordenador::definirSeed(int seed){
    this->seed = seed;
}