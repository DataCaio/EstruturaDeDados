#ifndef PACOTE_H
#define PACOTE_H
#include <string>

class Pacote{
    private:
        int chave;
        int alteracoes;
        std::string estado;
        int tempoArmazenado;
        int tempoTransportado;

    public:
        Pacote();
        ~Pacote();    

        int getChave();
        void setChave(int chave);

        void acrescentaTempoArmzenado(int tempo);
        int getTempoArmazenado();

        void acrescentaTempoTransportado(int tempo);
        int getTempoTransportado();

        void alteraEstado(std::string novoEstado);
        std::string getEstado();
        
    friend class pilha;
};

#endif