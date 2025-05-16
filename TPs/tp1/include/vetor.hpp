#ifndef MEUVETOR_HPP
#define MEUVETOR_HPP

template<typename T>
class MeuVetor {
private:
    T* dados;
    int capacidade;
    int tamanho;

    void redimensionar() {
        int novaCapacidade = capacidade * 2;
        T* novoArray = new T[novaCapacidade];
        for (int i = 0; i < tamanho; i++) {
            novoArray[i] = dados[i];
        }
        delete[] dados;
        dados = novoArray;
        capacidade = novaCapacidade;
    }

public:
    MeuVetor(int capInicial = 4)
        : capacidade(capInicial), tamanho(0) {
        dados = new T[capacidade];
    }

    ~MeuVetor() {
        delete[] dados;
    }

    void adicionar(const T& elemento) {
        if (tamanho >= capacidade) {
            redimensionar();
        }
        dados[tamanho++] = elemento;
    }

    T& operator[](int indice) {
        return dados[indice];
    }

    int tamanhoAtual() const {
        return tamanho;
    }

    void troca(int i, int j) {
        if (i >= 0 && i < tamanho && j >= 0 && j < tamanho) {
            T temp = dados[i];
            dados[i] = dados[j];
            dados[j] = temp;
        } else {
            throw std::out_of_range("Índice fora dos limites do vetor");
        }
    }

    int menorIndice() {
        if (tamanho == 0) {
            throw std::logic_error("Vetor vazio — não há menor elemento.");
        }
    
        int indiceMenor = 0;
        for (int i = 1; i < tamanho; ++i) {
            if (dados[i] < dados[indiceMenor]) {
                indiceMenor = i;
            }
        }
        return indiceMenor;
    }
    void atribuirValor(int i, const T& valor) {
        if (i >= 0 && i < tamanho) {
            dados[i] = valor;
        } else {
            throw std::out_of_range("Índice fora dos limites do vetor");
        }
    }


};

#endif
