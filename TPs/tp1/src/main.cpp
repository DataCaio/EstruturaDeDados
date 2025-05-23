#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include "ordenadorUniversal.hpp" 
MeuVetor<int> leArquivo(std::string nomeArquivo, int& seed, double& limiarCusto, double& a, double& b, double& c, int& tamanho) {
    int i;
    std::ifstream arquivo(nomeArquivo.c_str());
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        exit(1);
    }

    arquivo >> seed >> limiarCusto >> a >> b >> c >> tamanho;

    MeuVetor<int> vet = MeuVetor<int>(tamanho);
    int valor;
    for (i = 0; i < tamanho; i++) {
        arquivo >> valor;
        vet.adicionar(valor);
    }
    arquivo.close();
    return vet;
}


int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Uso: " << argv[0] << " <nome_arquivo>" << std::endl;
        return 1;
    }
    std::string nomeArquivo = (argc > 1) ? argv[1] : "";

    std::cout << std::fixed << std::setprecision(9);

    int seed, tam;
    double limiarCusto, a, b, c;

    MeuVetor<int> vet = leArquivo(nomeArquivo, seed, limiarCusto, a, b, c, tam);

    double constantes[3] = {a,b,c};

    Ordenador ordenador(vet,vet.tamanhoAtual(),0,0,constantes);

    std::cout << "size " << tam
        << " seed " << seed
        << " breaks " << ordenador.calculaQuebras()
        << std::endl;
    
    int limiarParticao = ordenador.determinaLimiarParticao(limiarCusto);
    int limiarQuebras = ordenador.determinaLimiarQuebras(limiarCusto,limiarParticao);
    
    Ordenador ideal(vet,vet.tamanhoAtual(),limiarParticao,limiarQuebras,constantes);
    std::cout << std::endl;

    return 0;
}