#include <iostream>
#include "heap.hpp"
#include "unionFind.hpp"

int main() {
    int n, m;
    std::cin >> n >> m;

    Heap heap(m); // no máximo m arestas
    UnionFind uf(n);

    // Inicializa os subconjuntos
    for (int i = 0; i < n; ++i) {
        uf.Make(i);
    }

    // Lê as arestas
    for (int i = 0; i < m; ++i) {
        int u, v, c;
        std::cin >> u >> v >> c;
        Aresta a;
        a.u = u;
        a.v = v;
        a.custo = c;
        heap.Inserir(a);
    }

    int custo_total = 0;
    int arestas_aceitas = 0;

    // Kruskal
    while (!heap.Vazio() && arestas_aceitas < n - 1) {
        Aresta a = heap.Remover();
        if (uf.Find(a.u) != uf.Find(a.v)) {
            uf.Union(a.u, a.v);
            custo_total += a.custo;
            arestas_aceitas++;
        }
    }

    std::cout << custo_total << std::endl;
    return 0;
}
