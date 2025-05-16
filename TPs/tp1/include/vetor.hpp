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

    int menorIndice(){

    }
};

#endif
