#include "../include/heap.hpp"
#include <iostream>

int main(){
    int maxsize;
    std::cin >> maxsize;
    Heap heap(maxsize);

    for(int i=0;i<maxsize;i++){
        int n;
        std::cin >> n;
        heap.Inserir(n); 
    }

    while (!heap.Vazio()) {
        std::cout << heap.Remover() << " ";
    }
    std::cout << std::endl;

}