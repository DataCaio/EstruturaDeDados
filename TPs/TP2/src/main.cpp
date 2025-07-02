#include <iostream>
#include <fstream> // --- PASSO 1: Incluir a biblioteca para arquivos
#include <string>

#include "grafo.hpp"
#include "armazem.hpp"
#include "transporte.hpp"
#include "escalonador.hpp"

// --- PASSO 2: Alterar a assinatura do main ---
int main(int argc, char* argv[]) {

    // --- PASSO 3: Verificar se o nome do arquivo foi passado ---
    if (argc < 2) {
        std::cerr << "Erro: Nenhum arquivo de entrada especificado!" << std::endl;
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << std::endl;
        return 1; // Retorna um código de erro
    }

    // --- PASSO 4: Abrir o arquivo ---
    std::ifstream arquivo_entrada(argv[1]);
    if (!arquivo_entrada.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << argv[1] << "'" << std::endl;
        return 1; // Retorna um código de erro
    }

    // --- A partir daqui, trocamos 'std::cin' por 'arquivo_entrada' ---

    int capacidade, latencia, intervalo, custoRemocao, numeroArmazens, numeroPacotes;

    arquivo_entrada >> capacidade;
    arquivo_entrada >> latencia;
    arquivo_entrada >> intervalo;
    arquivo_entrada >> custoRemocao;
    arquivo_entrada >> numeroArmazens;
    
    Transporte transporte(numeroArmazens);
    transporte.adicionaRotas(arquivo_entrada); // Modificamos para passar o arquivo como fonte

    Armazem* armazens = new Armazem[numeroArmazens]; 
    for (int i = 0; i < numeroArmazens; i++) {
        armazens[i].inicializa(numeroArmazens);
    }   

    arquivo_entrada >> numeroPacotes;

    // A capacidade do escalonador deve ser suficiente para todos os eventos
    // Uma estimativa segura pode ser (numPacotes * numArmazens) + (numArmazens * numArmazens)
    Escalonador escalonador(numeroPacotes * numeroArmazens * 2); 

    Pacote* pacotes = new Pacote[numeroPacotes];
    
    int tempoPrimeiraChegada = -1;

    for (int i = 0; i < numeroPacotes; i++) {
        int tempoChegada, chave_lida, origem, destino;
        std::string pac, org, dst;
        
        arquivo_entrada >> tempoChegada >> pac >> chave_lida >> org >> origem >> dst >> destino;

        // Atualiza o tempo da primeira chegada
        if (tempoPrimeiraChegada == -1 || tempoChegada < tempoPrimeiraChegada) {
            tempoPrimeiraChegada = tempoChegada;
        }

        pacotes[i] = Pacote(tempoChegada, i, origem, destino);
        pacotes[i].calcularMinhaRota(transporte.getGrafo());
        
        Evento chegadaPacote(tempoChegada, 1, i, origem);
        escalonador.insereEvento(chegadaPacote);
    }

    // 2. Agende os transportes iniciais baseados no tempo da primeira chegada.
    //    O tempo do primeiro transporte será: tempoPrimeiraChegada + intervalo
    if (tempoPrimeiraChegada != -1) {
         transporte.agendarTransportesIniciais(escalonador, tempoPrimeiraChegada + intervalo);
    }
    
    // Chamando o método que executa a simulação
    escalonador.inicializa(pacotes, numeroPacotes, armazens, numeroArmazens, transporte, latencia, intervalo, capacidade,custoRemocao);

    // Limpeza
    delete[] armazens;
    delete[] pacotes;
    arquivo_entrada.close(); // Boa prática fechar o arquivo
    return 0;
}