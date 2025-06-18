#ifndef EVENTO_H
#define EVENTO_H

class Evento{
    private:
        int tempo;
        int tipoEvento; /*1 caso pacote, 2 caso transporte*/
    public:
        Evento(int tempo, int tipoEvento);

        int getTempo();
        int getTipo();
        
};

#endif