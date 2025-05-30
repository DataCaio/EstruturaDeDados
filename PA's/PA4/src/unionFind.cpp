#include "unionFind.hpp"
#include <iostream>

UnionFind::UnionFind(int quantidade_subconjuntos){
    this->subconjuntos = new Subconjunto[quantidade_subconjuntos];
    this->tamanho = quantidade_subconjuntos;
}

UnionFind::~UnionFind(){
    delete[] this->subconjuntos;
}

void Make(int x){
    this->subconjunto
};