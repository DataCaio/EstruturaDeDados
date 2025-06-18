#ifndef CELULA_H
#define CELULA_H

#include "pacote.hpp"

class Celula {
private:
    Pacote pacote;
    Celula *prox;

public:
    // Construtor padrão (que você já tinha)
    Celula() {
        // Este construtor agora é menos útil, mas podemos manter.
        // Ele cria uma célula "vazia".
        prox = nullptr;
    }

    // --- CONSTRUTOR NOVO E CORRIGIDO AQUI ---
    // Este construtor recebe um Pacote e o armazena.
    // Usamos 'const Pacote& p' para ser eficiente e não criar cópias desnecessárias.
    Celula(const Pacote& p) : pacote(p), prox(nullptr) {
        // A sintaxe ': pacote(p)' chama o construtor de cópia do Pacote
        // para inicializar o membro 'pacote' com o 'p' que recebemos.
        // O corpo pode ficar vazio.
    }

    friend class Pilha;
};

#endif