#include "grafo.hpp"
#include "armazem.hpp"
#include "transporte.hpp"
#include "escalonador.hpp"
#include <iostream>

int main(){
    int capacidade, latencia,intervalo,custoRemocao,numeroArmazens,numeroPacotes;

    std::cin >> capacidade;
    std::cin >> latencia;
    std::cin >> intervalo;
    std::cin >> custoRemocao;
    std::cin >> numeroArmazens;
    
    Transporte transporte(numeroArmazens);
    transporte.adicionaRotas(); // Lê a matriz para o grafo

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
    
    Escalonador escalonador;
    transporte.agendarTransportesIniciais(escalonador, intervalo);
    

}