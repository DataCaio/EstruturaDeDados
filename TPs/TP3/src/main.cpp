#include <iostream>
#include <string>
#include <fstream>
#include <chrono> // Incluir para medição de tempo
#include "../include/logistica.hpp" 

int main(int argc, char* argv[]) {
    // Medição do tempo total de execução do programa
    auto start_total = std::chrono::high_resolution_clock::now();

    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << std::endl;
        return 1;
    }

    std::string nomeArquivoEntrada = argv[1];
    std::ifstream arquivoEntrada(nomeArquivoEntrada);

    if (!arquivoEntrada.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivoEntrada << std::endl;
        return 1;
    }

    Logistica sistema;

    std::string linha;
    
    // Medição do tempo de leitura do arquivo
    auto start_read_file = std::chrono::high_resolution_clock::now();
    while (std::getline(arquivoEntrada, linha)) {
        sistema.processarLinha(linha); // Chama o método para processar cada linha
    }
    auto end_read_file = std::chrono::high_resolution_clock::now();
    
    arquivoEntrada.close();

    // Calcular e imprimir tempos para o console de erro (stderr)
    auto end_total = std::chrono::high_resolution_clock::now();

    auto duration_read_file = std::chrono::duration_cast<std::chrono::microseconds>(end_read_file - start_read_file);
    auto duration_total = std::chrono::duration_cast<std::chrono::microseconds>(end_total - start_total);

    std::cerr << "TEMPO_LEITURA_ARQUIVO_US: " << duration_read_file.count() << std::endl;
    std::cerr << "TEMPO_PROCESSAMENTO_EV_US: " << sistema.getTotalEventProcessingTime() << std::endl; // Novo método
    std::cerr << "TEMPO_CONSULTA_CL_US: " << sistema.getTotalCLQueryTime() << std::endl; // Novo método
    std::cerr << "TEMPO_CONSULTA_PC_US: " << sistema.getTotalPCQueryTime() << std::endl; // Novo método
    std::cerr << "TEMPO_TOTAL_EXECUCAO_US: " << duration_total.count() << std::endl;

    return 0;
}