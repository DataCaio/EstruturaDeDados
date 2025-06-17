#ifndef FILA_H
#define FILA_H

// Nó específico para a Fila
struct NoFila {
    int item;
    NoFila* proximo;
};

class Fila {
private:
    NoFila* frente;
    NoFila* tras;

public:
    Fila();
    ~Fila();
    void enfileirar(int item);
    int desenfileirar();
    bool vazia();
};

#endif