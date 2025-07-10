#include "Set.hpp"

StringSet::StringSet(int tamanho) {
    this->tamanhoOriginal = tamanho;
    this->tamanhoTabela = tamanho;
    this->tamanhoConjunto = 0;
    this->tabela = new ElementoTabela[tamanhoTabela];

    for (int i = 0; i < tamanhoTabela; ++i) {
        tabela[i].vazio = true;
        tabela[i].retirada = false;
    }
}

StringSet::~StringSet() {
    delete[] tabela;
}

int StringSet::Hash(string s) {
    unsigned long hash = 0;
    for (char c : s) {
        hash = hash * 31 + c;
    }
    return hash % tamanhoTabela;
}

void StringSet::Resize(size_t novo_tamanho) {
    ElementoTabela* antiga = tabela;
    int tamanho_antigo = tamanhoTabela;

    tabela = new ElementoTabela[novo_tamanho];
    tamanhoTabela = novo_tamanho;
    tamanhoConjunto = 0;

    for (int i = 0; i < novo_tamanho; ++i) {
        tabela[i].vazio = true;
        tabela[i].retirada = false;
    }

    for (int i = 0; i < tamanho_antigo; ++i) {
        if (!antiga[i].vazio && !antiga[i].retirada) {
            Inserir(antiga[i].dado);
        }
    }

    delete[] antiga;
}

void StringSet::Rehash(int pos) {
    Resize(tamanhoTabela * 2);
}

void StringSet::Inserir(string s) {
    if (tamanhoConjunto * 2 >= tamanhoTabela) {
        Resize(tamanhoTabela * 2);
    }

    int h = Hash(s);
    int original_h = h;

    while (!tabela[h].vazio) {
        if (!tabela[h].retirada && tabela[h].dado == s) {
            return; // já está no conjunto
        }
        h = (h + 1) % tamanhoTabela;
        if (h == original_h) return; // evitamos loop infinito
    }

    tabela[h].dado = s;
    tabela[h].vazio = false;
    tabela[h].retirada = false;
    tamanhoConjunto++;
}

void StringSet::Remover(string s) {
    int h = Hash(s);
    int original_h = h;

    while (!tabela[h].vazio) {
        if (!tabela[h].retirada && tabela[h].dado == s) {
            tabela[h].retirada = true;
            tamanhoConjunto--;
            return;
        }
        h = (h + 1) % tamanhoTabela;
        if (h == original_h) return;
    }
}

bool StringSet::Pertence(string s) {
    int h = Hash(s);
    int original_h = h;

    while (!tabela[h].vazio) {
        if (!tabela[h].retirada && tabela[h].dado == s) {
            return true;
        }
        h = (h + 1) % tamanhoTabela;
        if (h == original_h) return false;
    }
    return false;
}

StringSet* StringSet::Uniao(StringSet* S) {
    StringSet* resultado = new StringSet(this->tamanhoConjunto + S->tamanhoConjunto);
    for (int i = 0; i < this->tamanhoTabela; i++) {
        if (!this->tabela[i].vazio && !this->tabela[i].retirada) {
            resultado->Inserir(this->tabela[i].dado);
        }
    }
    for (int i = 0; i < S->tamanhoTabela; i++) {
        if (!S->tabela[i].vazio && !S->tabela[i].retirada) {
            resultado->Inserir(S->tabela[i].dado);
        }
    }
    return resultado;
}

StringSet* StringSet::Intersecao(StringSet* S) {
    StringSet* resultado = new StringSet(min(this->tamanhoConjunto, S->tamanhoConjunto));
    for (int i = 0; i < this->tamanhoTabela; i++) {
        if (!this->tabela[i].vazio && !this->tabela[i].retirada) {
            if (S->Pertence(this->tabela[i].dado)) {
                resultado->Inserir(this->tabela[i].dado);
            }
        }
    }
    return resultado;
}

StringSet* StringSet::DiferencaSimetrica(StringSet* S) {
    StringSet* resultado = new StringSet(this->tamanhoConjunto + S->tamanhoConjunto);

    for (int i = 0; i < this->tamanhoTabela; i++) {
        if (!this->tabela[i].vazio && !this->tabela[i].retirada && !S->Pertence(this->tabela[i].dado)) {
            resultado->Inserir(this->tabela[i].dado);
        }
    }

    for (int i = 0; i < S->tamanhoTabela; i++) {
        if (!S->tabela[i].vazio && !S->tabela[i].retirada && !this->Pertence(S->tabela[i].dado)) {
            resultado->Inserir(S->tabela[i].dado);
        }
    }

    return resultado;
}

void StringSet::Imprimir() {
    // Aloca um array com o máximo possível de elementos válidos
    string* elementos = new string[tamanhoConjunto];
    int count = 0;

    for (int i = 0; i < tamanhoTabela; ++i) {
        if (!tabela[i].vazio && !tabela[i].retirada) {
            elementos[count++] = tabela[i].dado;
        }
    }

    // Ordenação simples (insertion sort) já que não podemos usar sort do STL
    for (int i = 1; i < count; ++i) {
        string key = elementos[i];
        int j = i - 1;
        while (j >= 0 && elementos[j] > key) {
            elementos[j + 1] = elementos[j];
            j--;
        }
        elementos[j + 1] = key;
    }

    cout << "{ ";
    for (int i = 0; i < count; ++i) {
        cout << elementos[i];
        if (i + 1 < count) cout << ", ";
    }
    cout << " }" << endl;

    delete[] elementos; // Libera memória alocada
}
