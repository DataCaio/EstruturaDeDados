#ifndef EVENTO_H
#define EVENTO_H
#include <string> 

class Evento {
private:
    int tempo;
    int tipoEvento; // 1: Chegada de Pacote, 2: Transporte

    struct Dados {
        int id_pacote;
        int local; // Usado tanto para local_chegada quanto para origem/destino
        int destino; // Usado apenas para transporte
    } dados;

    std::string chave; 
    void gera_chave();

public:
    // Construtor padrão (útil para inicialização)
    Evento(); 

    Evento(int tempo, int tipo, int arg1, int arg2);
    bool operator<(const Evento& outro) const;
    // Getters
    int getTempo() const;
    int getTipo() const;
    
    
    // Getters para dados específicos (nomes mais claros)
    int getIdPacote() const;
    int getLocalChegada() const;
    int getOrigemTransporte() const;
    int getDestinoTransporte() const;
    std::string getChave() const;
};

#endif