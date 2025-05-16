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
/* CONSERTAR E ENTENDER OS PARAMETROS DO LAÇO WHILE, E DESCOBRIR COMO PASSAR O ATRIBUTO LIMPARTICAO CORRETAMENTE: NAO PASSAR SOMENTE O INDICE, MAS O ELEMENTO */
void Ordenador::determinaLimiarParticao(float limiarCusto){
    int minMPS = 2;
    int maxMPS = tamanho;
    int passoMPS = (maxMPS - minMPS)/5;
    int numMPS = 0,limParticao = 0,newMin = 0,newMax = 0;
    float diffCusto = 0; 
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
        diffCusto = std::fabs(custo.operator[](newMin) - custo.operator[](newMax));
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
    int a = this->vetor.operator[](l);
    int b = this->vetor.operator[]((l+r)/2);
    int c = this->vetor.operator[](r);
    int p = median(a, b, c);
  
    do{
        while(p > this->vetor.operator[](*i)){
            (*i)++;this->estatisticas[2]++;
        }
        this->estatisticas[2]++;
        while(p < this->vetor.operator[](*j)){
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
      
      aux = vetor.operator[](i);
      this->estatisticas[3]++;
  
      j = i - 1;
  
      while(j>=l && vetor.operator[](j)){
        this->estatisticas[2];
        vetor.atribuirValor(j+1,vetor.operator[](j));
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

void Ordenador::determinaLimiarQuebras(int limiarCusto){}; /* IMPLEMENTAR */