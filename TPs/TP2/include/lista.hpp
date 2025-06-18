#ifndef LISTA_H
#define LISTA_H

// O nó da lista, que armazena o ID de um armazém
struct No {
    int id_armazem;
    No* proximo;

    No(int id) : id_armazem(id), proximo(nullptr) {}
};


// A classe que gerencia a lista de nós
class ListaEncadeada {
private:
    No* cabeca;
    int tamanho;

public:
    ListaEncadeada();
    ListaEncadeada(const ListaEncadeada& outra);
    ~ListaEncadeada(); // Destrutor para limpar a memória

    void insereNoInicio(int id_armazem);
    void exibir();
    
    No* getCabeca() const; // Getter para a cabeça da lista

};

#endif