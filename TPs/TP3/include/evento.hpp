#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>
#include <sstream> // For parsing within Evento constructors if needed, or stringstream for current state

// Enum for event types for better readability and type safety
enum TipoEvento {
    RG, // Pacote ainda não foi postado (apenas registrado no sistema)
    AR, // Pacote chegou no armazém e foi armazenado na respectiva seção
    RM, // Pacote foi removido da seção para tentativa de transporte
    UR, // Pacote foi rearmazenado por não ser possível transportá-lo
    TR, // Pacote sendo transportado entre armazéns
    EN, // Pacote entregue
    INVALID_EVENT_TYPE // For error handling
};

// Function to convert string to TipoEvento
inline TipoEvento stringToTipoEvento(const std::string& s) {
    if (s == "RG") return RG;
    if (s == "AR") return AR;
    if (s == "RM") return RM;
    if (s == "UR") return UR;
    if (s == "TR") return TR;
    if (s == "EN") return EN;
    return INVALID_EVENT_TYPE;
}

// Base class for all events
struct EventoBase {
    int dataHora;       // Data Hora (Field 1)
    TipoEvento tipo;    // Tipo evento
    int idPacote;       // ID Pacote (Field 4)
    std::string linhaOriginal; // To store the exact original input line for PC queries

    // Virtual destructor for proper polymorphic cleanup
    virtual ~EventoBase() = default;

    EventoBase(int dh, TipoEvento t, int id, const std::string& original)
        : dataHora(dh), tipo(t), idPacote(id), linhaOriginal(original) {}

    // Virtual function to get the state string for CL queries
    virtual std::string getEstadoAtual() const = 0;
};

// Derived structs for specific event types
struct EventoRG : public EventoBase {
    std::string remetente;  // Remetente (Field 5)
    std::string destinatario; // Destinatário (Field 6)
    int armazemOrigem;      // Armazém Origem (Field 7)
    int armazemDestino;     // Armazém Destino (Field 8)

    EventoRG(int dh, int id, const std::string& rem, const std::string& dest,
             int armOrig, int armDest, const std::string& original)
        : EventoBase(dh, RG, id, original), remetente(rem), destinatario(dest),
          armazemOrigem(armOrig), armazemDestino(armDest) {}
    
    std::string getEstadoAtual() const override { return "registrado"; }
};

struct EventoAR : public EventoBase {
    int armazemDestino; // Armazém Destino (Field 5)
    int secaoDestino;   // Seção Destino (Field 6)

    EventoAR(int dh, int id, int armDest, int secDest, const std::string& original)
        : EventoBase(dh, AR, id, original), armazemDestino(armDest), secaoDestino(secDest) {}
    
    std::string getEstadoAtual() const override {
        return "armazenado no armazém " + std::to_string(armazemDestino) + ", seção " + std::to_string(secaoDestino);
    }
};

struct EventoRM : public EventoBase {
    int armazemOrigem;  // Armazém Origem (Field 5)
    int secaoDestino;   // Seção Destino (Field 6) // This field is "secao_origem" according to typical RM events

    EventoRM(int dh, int id, int armOrig, int secDest, const std::string& original)
        : EventoBase(dh, RM, id, original), armazemOrigem(armOrig), secaoDestino(secDest) {} // secaoDestino should be secaoOrigem
    
    std::string getEstadoAtual() const override {
        return "removido do armazém " + std::to_string(armazemOrigem);
    }
};

struct EventoUR : public EventoBase {
    int armazemDestino; // Armazém Destino (Field 5)
    int secaoDestino;   // Seção Destino (Field 6)

    EventoUR(int dh, int id, int armDest, int secDest, const std::string& original)
        : EventoBase(dh, UR, id, original), armazemDestino(armDest), secaoDestino(secDest) {}
    
    std::string getEstadoAtual() const override {
        return "rearmazenado no armazém " + std::to_string(armazemDestino) + ", seção " + std::to_string(secaoDestino);
    }
};

struct EventoTR : public EventoBase {
    int armazemOrigem;  // Armazém Origem (Field 5)
    int armazemDestino; // Armazém Destino (Field 6)

    EventoTR(int dh, int id, int armOrig, int armDest, const std::string& original)
        : EventoBase(dh, TR, id, original), armazemOrigem(armOrig), armazemDestino(armDest) {}
    
    std::string getEstadoAtual() const override {
        return "em transporte entre armazéns " + std::to_string(armazemOrigem) + " e " + std::to_string(armazemDestino);
    }
};

struct EventoEN : public EventoBase {
    int armazemDestino; // Armazém Destino (Field 5) (This is often the final destination, not a current warehouse)

    EventoEN(int dh, int id, int armDest, const std::string& original)
        : EventoBase(dh, EN, id, original), armazemDestino(armDest) {}
    
    std::string getEstadoAtual() const override { return "entregue"; }
};

#endif // EVENTO_HPP