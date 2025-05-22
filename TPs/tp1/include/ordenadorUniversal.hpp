#ifndef ORDENADOR_H
#define ORDENADOR_H
#include "vetor.hpp"

class Ordenador{
    private:
         MeuVetor<int> vetor;
         int tamanho;
         int limiarParticao;
         int limiarQuebras;
         float custo;
         int estatisticas[4] = {0,0,0,0}; /* mps/qps,comparações,movimentações,chamadas */
         double constantes[3] = {0,0,0}; /* a,b,c */
    public:
        Ordenador(const MeuVetor<int>& vetor,int tamanho,int limiarParticao
            ,int limiarQuebras, double constantes[3]);
    
        void ordenacao();
        
        int determinaLimiarParticao(double limiarCusto);

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