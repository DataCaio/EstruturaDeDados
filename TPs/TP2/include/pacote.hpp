#ifndef PACOTE_H
#define PACOTE_H
#include <string>

class Pacote{
    private:
        int chave;
        int alteracoes; /*!!*/
        std::string estado;

        int origem;
        int destino;

        int tempoArmazenado;
        int tempoTransportado;
        int tempoChegada;

    public:
        Pacote(int tempoChegada,int chave,int origem, int destino);
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