// main.cpp
#include <iostream>
#include <string>
#include "../include/graph.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return 1;
  }
  std::string op = argv[1];

  int n;
  if (!(std::cin >> n)) return 1;

  Grafo g;
  for (int i = 0; i < n; ++i) g.InsereVertice();

  for (int i = 0; i < n; ++i) {
    int m;
    std::cin >> m;
    for (int j = 0; j < m; ++j) {
      int w;
      std::cin >> w;
      g.InsereAresta(i, w);
    }
  }

  if (op == "-d") {
    std::cout << g.QuantidadeVertices() << "\n";
    std::cout << g.QuantidadeArestas() << "\n";
    std::cout << g.GrauMinimo() << "\n";
    std::cout << g.GrauMaximo() << "\n";
  } else if (op == "-n") {
    for (int i = 0; i < g.QuantidadeVertices(); ++i) {
      g.ImprimeVizinhos(i);
    }
  } else if (op == "-k") {
    int nV = g.QuantidadeVertices();
    int minG = g.GrauMinimo();
    std::cout << (minG == nV - 1 ? "1\n" : "0\n");
  } else {
    return 1;
  }

  return 0;
}
