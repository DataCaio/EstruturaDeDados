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


    // Cria o Escalonador. SÓ EXISTE UM para toda a simulação.
    Escalonador escalonador;// Inicializa o relógio e o estado do escalonador. Essencial!
    
    Armazem* armazens = new Armazem[numeroArmazens]; 

    std::cin >> numeroPacotes;
    Pacote* pacotes = new Pacote[numeroPacotes];
    
    for(int i=0;i<numeroPacotes;i++){
        int tempoChegada,chave,origem,destino;
        std::string pac,org,dst;
        std::cin >> tempoChegada; 
        std::cin >> pac >> chave;
        std::cin >> org >> origem;
        std::cin >> dst >> destino;

        pacotes[i] = Pacote(tempoChegada,chave,origem,destino);
        
        pacotes[i].calcularMinhaRota(transporte.getGrafo());
        Evento chegadaPacote(tempoChegada, 1, chave);
        escalonador.insereEvento(chegadaPacote);
    }

    transporte.agendarTransportesIniciais(escalonador, intervalo);
    escalonador.inicializa(pacotes, numeroPacotes, armazens, numeroArmazens, transporte, latencia, intervalo, capacidade);


}