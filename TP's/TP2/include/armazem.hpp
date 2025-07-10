#ifndef ARMAZEM_H
#define ARMAZEM_H

#include "pilha.hpp"
#include "pacote.hpp"

class Armazem {
private:
    // Um ponteiro para um array de Pilhas. Cada pilha é uma seção.
    Pilha* secoes;
    int num_secoes;

public:
    // Construtor e Destrutor
    Armazem();
    ~Armazem();

    // Método para inicializar o armazém com o número correto de seções
    void inicializa(int numTotalArmazens);

    // Armazena um pacote na seção correta (pilha do próximo destino)
    void armazena(Pacote& pacote, int proximo_destino_na_rota);

    // Recupera um pacote da seção correta (pilha do destino do transporte)
    Pacote recupera(int destino_transporte);

    // Verifica se uma seção específica tem pacotes para transportar
    bool temPacotesPara(int destino_transporte) const;
};

#endif