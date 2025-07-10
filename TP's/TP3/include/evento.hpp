#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>
#include <sstream>

enum TipoEvento {
    RG, // Pacote ainda não foi postado (apenas registrado no sistema)
    AR, // Pacote chegou no armazém e foi armazenado na respectiva seção
    RM, // Pacote foi removido da seção para tentativa de transporte
    UR, // Pacote foi rearmazenado por não ser possível transportá-lo
    TR, // Pacote sendo transportado entre armazéns
    EN, // Pacote entregue
    INVALID_EVENT_TYPE // CASO ERRO
};

// converte string para tipo evento
inline TipoEvento stringToTipoEvento(const std::string& s) {
    if (s == "RG") return RG;
    if (s == "AR") return AR;
    if (s == "RM") return RM;
    if (s == "UR") return UR;
    if (s == "TR") return TR;
    if (s == "EN") return EN;
    return INVALID_EVENT_TYPE;
}

struct EventoBase {
    int tempo;
    TipoEvento tipo;
    int idPacote;
    std::string linhaOriginal;

    virtual ~EventoBase() = default;

    EventoBase(int dh, TipoEvento t, int id, const std::string& original)
        : tempo(dh), tipo(t), idPacote(id), linhaOriginal(original) {}
};

struct EventoRG : public EventoBase {
    std::string remetente;
    std::string destinatario;
    int armazemOrigem;
    int armazemDestino;

    EventoRG(int dh, int id, const std::string& rem, const std::string& dest,
             int armOrig, int armDest, const std::string& original)
        : EventoBase(dh, RG, id, original), remetente(rem), destinatario(dest),
          armazemOrigem(armOrig), armazemDestino(armDest) {}
};

struct EventoAR : public EventoBase {
    int armazemDestino;
    int secaoDestino;

    EventoAR(int dh, int id, int armDest, int secDest, const std::string& original)
        : EventoBase(dh, AR, id, original), armazemDestino(armDest), secaoDestino(secDest) {}
};

struct EventoRM : public EventoBase {
    int armazemOrigem;
    int secaoDestino;
    EventoRM(int dh, int id, int armOrig, int secDest, const std::string& original)
        : EventoBase(dh, RM, id, original), armazemOrigem(armOrig), secaoDestino(secDest) {}
};

struct EventoUR : public EventoBase {
    int armazemDestino;
    int secaoDestino;
    EventoUR(int dh, int id, int armDest, int secDest, const std::string& original)
        : EventoBase(dh, UR, id, original), armazemDestino(armDest), secaoDestino(secDest) {}

};

struct EventoTR : public EventoBase {
    int armazemOrigem;
    int armazemDestino;

    EventoTR(int dh, int id, int armOrig, int armDest, const std::string& original)
        : EventoBase(dh, TR, id, original), armazemOrigem(armOrig), armazemDestino(armDest) {}
};

struct EventoEN : public EventoBase {
    int armazemDestino;
    EventoEN(int dh, int id, int armDest, const std::string& original)
        : EventoBase(dh, EN, id, original), armazemDestino(armDest) {}
};

#endif // EVENTO_HPP