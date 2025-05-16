#ifndef ORDENADOR_H
#define ORDENADOR_H
#include "vetor.hpp"

class Ordenador{
    private:
         MeuVetor<int> vetor;
         int tamanho;
         int limiarParticao;
         int limiarQuebras;
         int estatisticas[5]; 
    public:
        Ordenador(MeuVetor<int> vetor,int tamanho,int limiarParticao
            ,int limiarQuebras);
    
        void ordenacao();
        
        void determinaLimiarParticao(int limiarCusto);

        void calculaNovaFaixa(int limParticao,int minMPS, 
            int maxMPS,int passoMPS);

        int determinaLimiarQuebras(MeuVetor<int>,int tamanho,
            int limiarCusto);

        int registraEstatisticas(int mps, int cost, int cmp, 
            int move,int calls);

        void imprimeEstatisticas();

        void quickSort();

        void insercao();

};

#endif