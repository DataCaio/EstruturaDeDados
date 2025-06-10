#include "grafo.hpp"
#include "armazem.hpp"
#include <iostream>

int main(){
    int capacidade, latencia,intervalo,custoRemocao,numeroArmzaens,numeroPacotes;

    std::cin >> capacidade;
    std::cin >> latencia;
    std::cin >> intervalo;
    std::cin >> custoRemocao;
    std::cin >> numeroArmzaens;
    
    Grafo armazens(numeroArmzaens);
    armazens.lerMatriz();

    std::cin >> numeroPacotes;
    Pacote pacotes[numeroPacotes];
    for(int i=0;i<numeroPacotes;i++){
        int tempoChegada,chave,origem,destino;
        std::string pac,org,dst;
        std::cin >> tempoChegada; 
        std::cin >> pac >> chave;
        std::cin >> org >> origem;
        std::cin >> dst >> destino;

        Pacote pacote(tempoChegada,chave,origem,destino);
        pacotes[i] = pacote;
    }


}