#ifndef ARVOREAVL_HPP
#define ARVOREAVL_HPP

#include <cstddef>     // For nullptr
#include <algorithm>   // For std::max (alternatively use your maximo function)
#include <iostream>    // For std::cout in emOrdem (for debugging)

// Helper function (can be global or member of the class)
int maximo(int a, int b){
    return (a > b) ? a : b;
}

template <typename K, typename V> // Added K for Key and V for Value
class ArvoreAVL {
    private:
        struct No {
            K chave;    // The key for comparison
            V valor;    // The value associated with the key
            No* esquerdo;
            No* direito;
            int altura;

            No(const K& c, const V& v) : chave(c), valor(v), esquerdo(nullptr), direito(nullptr), altura(1) {}
        };

        No* raiz;
        int tamanho;

        // Retorna altura do nó ou 0 se nulo
        int altura(No* no) const {
            return no ? no->altura : 0;
        }

        // Fator de balanceamento
        int fatorBalanceamento(No* no) const {
            return no ? altura(no->esquerdo) - altura(no->direito) : 0;
        }

        void atualizarAltura(No* no) {
            no->altura = 1 + maximo(altura(no->esquerdo), altura(no->direito));     
        }

        // Rotação simples à direita
        No* rotacaoDireita(No* y) {
            No* x = y->esquerdo;
            No* T2 = x->direito;

            x->direito = y;
            y->esquerdo = T2;

            atualizarAltura(y);
            atualizarAltura(x);

            return x;
        }

        // Rotação simples à esquerda
        No* rotacaoEsquerda(No* x) {
            No* y = x->direito;
            No* T2 = y->esquerdo;

            y->esquerdo = x;
            x->direito = T2;

            atualizarAltura(x);
            atualizarAltura(y);

            return y;
        }

        // Balanceamento após inserção
        No* balancear(No* no, const K& chave) { // Balance based on the key
            atualizarAltura(no);
            int fb = fatorBalanceamento(no);

            // Caso esquerda-esquerda (LL)
            if (fb > 1 && chave < no->esquerdo->chave) // Compare with key
                return rotacaoDireita(no);

            // Caso direita-direita (RR)
            if (fb < -1 && chave > no->direito->chave) // Compare with key
                return rotacaoEsquerda(no);

            // Caso esquerda-direita (LR)
            if (fb > 1 && chave > no->esquerdo->chave) { // Compare with key
                no->esquerdo = rotacaoEsquerda(no->esquerdo);
                return rotacaoDireita(no);
            }

            // Caso direita-esquerda (RL)
            if (fb < -1 && chave < no->direito->chave) { // Compare with key
                no->direito = rotacaoDireita(no->direito);
                return rotacaoEsquerda(no);
            }

            return no;
        }

        // Inserção recursiva (agora com chave e valor)
        No* inserir(No* no, const K& chave, const V& valor) {
            if (!no) {
                tamanho++;
                return new No(chave, valor); // Create node with key and value
            }

            if (chave < no->chave) // Compare with key
                no->esquerdo = inserir(no->esquerdo, chave, valor);
            else if (chave > no->chave) // Compare with key
                no->direito = inserir(no->direito, chave, valor);
            else {
                // Se a chave já existe, atualiza o valor
                no->valor = valor; 
                return no;
            }

            return balancear(no, chave); // Balance based on the key
        }

        // Busca recursiva (retorna ponteiro para o valor)
        V* busca(No* no, const K& chave) const {
            if (!no) {
                return nullptr; // Chave não encontrada
            }

            if (chave < no->chave) {
                return busca(no->esquerdo, chave);
            } else if (chave > no->chave) {
                return busca(no->direito, chave);
            } else {
                return &(no->valor); // Chave encontrada, retorna ponteiro para o valor
            }
        }

        // Impressão In-Order (recursiva) - para depuração
        void emOrdem(No* no) const {
            if (no) {
                emOrdem(no->esquerdo);
                // Assume que V tem um operador << ou pode ser impresso diretamente
                std::cout << "(" << no->chave << ": " << no->valor << ") "; 
                emOrdem(no->direito);
            }
        }

        // Libera memória recursivamente
        void destruir(No* no) {
            if (no) {
                destruir(no->esquerdo);
                destruir(no->direito);
                delete no;
            }
        }

    public:
        ArvoreAVL() : raiz(nullptr), tamanho(0) {}

        ~ArvoreAVL() {
            destruir(raiz);
        }

        // Método público para inserção
        void inserir(const K& chave, const V& valor) {
            raiz = inserir(raiz, chave, valor);
        }

        // Método público para busca
        V* busca(const K& chave) const {
            return busca(raiz, chave);
        }

        void imprimirEmOrdem() const {
            emOrdem(raiz);
            std::cout << std::endl;
        }

        int obterTamanho() const {
            return tamanho;
        }

        int obterAltura() const {
            return altura(raiz);
        }
};

#endif