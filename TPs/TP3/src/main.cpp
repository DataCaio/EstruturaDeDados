#include <iostream>
#include <string>
#include <fstream>
#include "../include/logistica.hpp" 

int main(int argc, char* argv[]) {
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
    while (std::getline(arquivoEntrada, linha)) {
        sistema.processarLinha(linha); // Chama o método para processar cada linha
    }

    arquivoEntrada.close();
    return 0;
}