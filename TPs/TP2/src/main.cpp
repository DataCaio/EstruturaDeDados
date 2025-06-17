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
        
        pacote.calcularMinhaRota(transporte.getGrafo());
        Evento chegadaPacote(tempoChegada, 1, chave);
        escalonador.insereEvento(chegadaPacote);

        pacotes[i] = pacote;
    }
    
    Escalonador escalonador;
    transporte.agendarTransportesIniciais(escalonador, intervalo);
    
    while (!escalonador.vazio()) { // Enquanto houver eventos na agenda
    
    // 1. Pega o próximo evento em ordem cronológica
    Evento proximo_evento = escalonador.retiraEvento();

    // 2. Avança o relógio da simulação (o retiraEvento pode fazer isso)
    int tempoAtual = proximo_evento.getTempo();

    // 3. Verifica o tipo do evento e age de acordo
    if (proximo_evento.getTipo() == 1) { // É uma Chegada de Pacote
        
        // --- Lógica para Chegada de Pacote ---
        // 1. Encontre o pacote correspondente ao evento.
        // 2. Verifique se o armazém atual é o destino final do pacote.
        [cite_start]// 3. Se for, registre a entrega. [cite: 87]
        [cite_start]// 4. Se não for, armazene o pacote na seção correta do armazém atual. [cite: 87]

    } else if (proximo_evento.getTipo() == 2) { // É um Transporte
        
        // --- Lógica para Transporte ---
        // 1. Identifique a rota do transporte (origem e destino).
        // 2. Vá para o armazém de origem e acesse a pilha da seção correta.
        // 3. Remova pacotes da pilha (respeitando a capacidade). Para cada um:
        //      a. [cite_start]Crie um novo evento de "Chegada de Pacote" no destino. [cite: 87]
        //      b. O tempo desse novo evento será: tempoAtual + latencia.
        //      c. Insira esse novo evento no escalonador.
        [cite_start]// 4. Agende o próximo evento de transporte para esta mesma rota (tempoAtual + intervalo). [cite: 87]
    }
}

// --- Fim do Loop ---

[cite_start]// 4. Gerar relatórios de estatísticas aqui. [cite: 87]
std::cout << "Fim da simulacao." << std::endl
    

}