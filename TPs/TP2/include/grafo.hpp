#ifndef GRAPH_H
#define GRAPH_H

class Grafo{
private:
    int** matriz;
    int numVertices;

public:
    Grafo(int vertices);

    ~Grafo();

    void lerMatriz();

    void exibir() {
        std::cout << "\nMatriz de Adjacência:\n";
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                std::cout << matriz[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
};


#endif