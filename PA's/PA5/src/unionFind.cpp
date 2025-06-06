#include "unionFind.hpp"
#include <iostream>

UnionFind::UnionFind(int quantidade_subconjuntos){
    this->subconjuntos = new Subconjunto[quantidade_subconjuntos];
    this->tamanho = quantidade_subconjuntos;
}

UnionFind::~UnionFind(){
    delete[] this->subconjuntos;
}
void UnionFind::Make(int x) {
    subconjuntos[x].representante = x;
    subconjuntos[x].rank = 0;
}

int UnionFind::Find(int x) {
    if (subconjuntos[x].representante != x) {
        subconjuntos[x].representante = Find(subconjuntos[x].representante);
    }
    return subconjuntos[x].representante;
}

void UnionFind::Union(int x, int y){
    int raiz_x = Find(x);
    int raiz_y = Find(y);
    if(raiz_x != raiz_y){
	    if(subconjuntos[raiz_x].rank < subconjuntos[raiz_y].rank){
		    subconjuntos[raiz_x] = subconjuntos[raiz_y];
	    } 
        else if(subconjuntos[raiz_x].rank > subconjuntos[raiz_y].rank){
		    subconjuntos[raiz_y] = subconjuntos[raiz_x];
	    }    
        else {
		    subconjuntos[raiz_y].representante = raiz_x;
		    subconjuntos[raiz_x].rank++;
	    }
	}

}