#ifndef VETORDINAMICO_HPP
#define VETORDINAMICO_HPP

#include <iostream>
#include <stdexcept>

template <typename T>
class VetorDinamico {
private:
    T* dados;
    int capacidade;
    int tamanho;

    // Redimensiona o vetor dobrando a capacidade
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
    VetorDinamico(int capacidadeInicial = 4)
        : capacidade(capacidadeInicial), tamanho(0) {
        dados = new T[capacidade];
    }

    ~VetorDinamico() {
        delete[] dados;
    }

    // Adiciona um elemento no final
    void insereFinal(const T& valor) {
        if (tamanho == capacidade) {
            redimensionar();
        }
        dados[tamanho++] = valor;
    }

    // Remove o último elemento
    void removeFinal() {
        if (tamanho > 0) {
            tamanho--;
        }
    }

    T& operator[](int indice) const{
        return dados[indice];
    }

    // Retorna o tamanho atual
    int size() const {
        return tamanho;
    }

    // Retorna a capacidade total
    int capacity() const {
        return capacidade;
    }

    // Imprime todos os elementos
    void exibir() const {
        for (int i = 0; i < tamanho; i++) {
            std::cout << dados[i] << " ";
        }
        std::cout << "\n";
    }
};

#endif
