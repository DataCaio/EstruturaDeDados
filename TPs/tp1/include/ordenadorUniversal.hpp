#ifndef ORDENADOR_H
#define ORDENADOR_H
#include "vetor.hpp"

class Ordenador{
    private:
         MeuVetor<int> vetor;
         int tamanho;
         int limiarParticao;
         int limiarQuebras;
         int estatisticas[5] = {0,0,0,0,0}; /* mps/qps,custo,comparações,movimentações,chamadas */
         int constantes[3] = {0,0,0}; /* a,b,c */
    public:
        Ordenador(MeuVetor<int> vetor,int tamanho,int limiarParticao
            ,int limiarQuebras, int constantes[3]);
    
        void ordenacao();
        
        int determinaLimiarParticao(float limiarCusto);

        int determinaLimiarQuebras(int limiarCusto,int limiarParticao);

        void imprimeEstatisticas();

        void quickSort3Ins(int l,int r);
        void partition3(int l,int r, int *i,int *j);

        void insercao(int l, int r);

        void suffleVector(int numShuffle);

        void resetaEstatisticas();

        void calculaCusto();
        int calculaQuebras();

};

#endif