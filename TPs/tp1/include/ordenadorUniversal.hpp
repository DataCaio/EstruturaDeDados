#ifndef ORDENADOR_H
#define ORDENADOR_H
#include "vetor.hpp"

class Ordenador{
    private:
         MeuVetor<int> vetor;
         int tamanho;
         int limiarParticao;
         int limiarQuebras;
         int estatisticas[5] = {0,0,0,0,0}; /* mps,custo,comparações,movimentações,chamadas */
    public:
        Ordenador(MeuVetor<int> vetor,int tamanho,int limiarParticao
            ,int limiarQuebras);
    
        void ordenacao();
        
        void determinaLimiarParticao(float limiarCusto);

        void determinaLimiarQuebras(int limiarCusto);

        void imprimeEstatisticas();

        void quickSort3Ins(int l,int r);
        void partition3(int l,int r, int *i,int *j);

        void insercao(int l, int r);

};

#endif