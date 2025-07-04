#include "../include/logistica.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>
#include "mergeSort.hpp"

struct ComparadorEventos {
    bool operator()(EventoBase* a, EventoBase* b) const {
        if (a->dataHora != b->dataHora)
            return a->dataHora < b->dataHora;
        return a->idPacote < b->idPacote;
    }
};

Logistica::Logistica() {
}

Logistica::~Logistica() {
    // Libera a memória alocada para os objetos EventoBase
    for (int i = 0; i < eventos.size(); ++i) {
        delete eventos[i];
    }
}



// Método auxiliar privado para criar o evento
EventoBase* Logistica::criarEventoDeLinha(const std::string& linha) {
    std::istringstream iss(linha);
    int dataHora, idPacote;
    std::string tipoLinha, tipoEventoStr;

    iss >> dataHora >> tipoLinha >> tipoEventoStr >> idPacote;

    TipoEvento tipoEvento = stringToTipoEvento(tipoEventoStr);
    
    // Movendo a lógica de criação de eventos para cá
    if (tipoEvento == RG) {
        std::string remetente, destinatario;
        int armOrig, armDest;
        iss >> remetente >> destinatario >> armOrig >> armDest;
        return new EventoRG(dataHora, idPacote, remetente, destinatario, armOrig, armDest, linha);
    } else if (tipoEvento == AR) {
        int armDest, secDest;
        iss >> armDest >> secDest;
        return new EventoAR(dataHora, idPacote, armDest, secDest, linha);
    } else if (tipoEvento == RM) {
        int armOrig, secOrig;
        iss >> armOrig >> secOrig;
        return new EventoRM(dataHora, idPacote, armOrig, secOrig, linha);
    } else if (tipoEvento == UR) {
        int armDest, secDest;
        iss >> armDest >> secDest;
        return new EventoUR(dataHora, idPacote, armDest, secDest, linha);
    } else if (tipoEvento == TR) {
        int armOrig, armDest;
        iss >> armOrig >> armDest;
        return new EventoTR(dataHora, idPacote, armOrig, armDest, linha);
    } else if (tipoEvento == EN) {
        int armDest;
        iss >> armDest;
        return new EventoEN(dataHora, idPacote, armDest, linha);
    }
    return nullptr; // Tipo de evento inválido
}

// Método auxiliar privado para atualizar os índices
void Logistica::atualizarIndices(EventoBase* novoEvento, int eventIndex) {
    int idPacote = novoEvento->idPacote;
    TipoEvento tipoEvento = novoEvento->tipo;

    // Lógica para atualizar indiceClientes (extraída de processaEvento)
    if (tipoEvento == RG) {
        EventoRG* rgEvent = static_cast<EventoRG*>(novoEvento);
        
        Cliente* remetenteCliente = indiceClientes.busca(rgEvent->remetente);
        if (!remetenteCliente) {
            Cliente novoCliente(rgEvent->remetente);
            novoCliente.pacotes.adicionaNoFim(idPacote);
            indiceClientes.inserir(rgEvent->remetente, novoCliente);
        } else {
            if (!remetenteCliente->pacotes.contains(idPacote)) {
                remetenteCliente->pacotes.adicionaNoFim(idPacote);
            }
        }
        
        Cliente* destinatarioCliente = indiceClientes.busca(rgEvent->destinatario);
        if (!destinatarioCliente) {
            Cliente novoCliente(rgEvent->destinatario);
            novoCliente.pacotes.adicionaNoFim(idPacote);
            indiceClientes.inserir(rgEvent->destinatario, novoCliente);
        } else {
            if (!destinatarioCliente->pacotes.contains(idPacote)) {
                destinatarioCliente->pacotes.adicionaNoFim(idPacote);
            }
        }
    }

    // Lógica para atualizar indicePacotes (extraída de processaEvento)
    Pacote* pacoteAtual = indicePacotes.busca(idPacote);
    if (!pacoteAtual) {
        Pacote novoPacote(idPacote);
        novoPacote.linhaRG = eventIndex;
        novoPacote.linhaUltimo = eventIndex;
        if (tipoEvento == RG) {
            EventoRG* rgEvent = static_cast<EventoRG*>(novoEvento);
            novoPacote.remetente = rgEvent->remetente;
            novoPacote.destinatario = rgEvent->destinatario;
        }
        novoPacote.historico.adicionaNoFim(eventIndex);
        indicePacotes.inserir(idPacote, novoPacote);
    } else {
        pacoteAtual->linhaUltimo = eventIndex;
        pacoteAtual->historico.adicionaNoFim(eventIndex);
    }
}

// Método principal para processar qualquer linha de entrada
void Logistica::processarLinha(const std::string& linha) {
    std::istringstream iss(linha);
    int dataHora;
    std::string tipoLinha;

    if (!(iss >> dataHora >> tipoLinha)) {
        return; 
    }

    if (tipoLinha == "EV") {
        EventoBase* novoEvento = criarEventoDeLinha(linha);
        if (novoEvento) {
            eventos.insereFinal(novoEvento);
            int eventIndex = eventos.size() - 1;
            atualizarIndices(novoEvento, eventIndex);
        }
    } else if (tipoLinha == "CL") {
        std::string nomeCliente;
        iss >> nomeCliente;
        processarConsultaCL(dataHora, nomeCliente);
    } else if (tipoLinha == "PC") {
        int idPacote;
        iss >> idPacote;
        processarConsultaPC(dataHora, idPacote);
    }
}

// Lógica de consulta CL 
void Logistica::processarConsultaCL(int dataHoraConsulta, const std::string& nomeCliente) {
    std::cout << std::setw(6) << std::setfill('0') << dataHoraConsulta << " CL " << nomeCliente << std::endl;

    VetorDinamico<EventoBase*> eventosParaRespostaCL;

    Cliente* clienteBuscado = indiceClientes.busca(nomeCliente);
    if (clienteBuscado) {
        for (int i = 0; i < clienteBuscado->pacotes.size(); ++i) {
            int idPacote = clienteBuscado->pacotes.getElemento(i);
            Pacote* pacoteInfo = indicePacotes.busca(idPacote);
            
            if (pacoteInfo) {
                EventoRG* rgEvent = static_cast<EventoRG*>(eventos[pacoteInfo->linhaRG]);
                if (rgEvent && rgEvent->dataHora <= dataHoraConsulta) {
                    if (rgEvent->remetente == nomeCliente || rgEvent->destinatario == nomeCliente) {
                        bool alreadyAdded = false;
                        for(int k=0; k<eventosParaRespostaCL.size(); ++k) {
                            if (eventosParaRespostaCL[k] == rgEvent) {
                                alreadyAdded = true;
                                break;
                            }
                        }
                        if (!alreadyAdded) {
                            eventosParaRespostaCL.insereFinal(rgEvent);
                        }
                    }
                }

                EventoBase* lastNonRGEvent = nullptr;
                for (int j = 0; j < pacoteInfo->historico.size(); ++j) {
                    int eventIndex = pacoteInfo->historico.getElemento(j);
                    EventoBase* currentEvent = eventos[eventIndex];
                    
                    if (currentEvent->dataHora <= dataHoraConsulta) {
                        if (currentEvent->tipo != RG) {
                            if (!lastNonRGEvent || currentEvent->dataHora > lastNonRGEvent->dataHora) {
                                lastNonRGEvent = currentEvent;
                            } else if (currentEvent->dataHora == lastNonRGEvent->dataHora) {
                                lastNonRGEvent = currentEvent; 
                            }
                        }
                    }
                }
                if (lastNonRGEvent) {
                     bool alreadyAdded = false;
                     for (int k = 0; k < eventosParaRespostaCL.size(); ++k) {
                         if (eventosParaRespostaCL[k] == lastNonRGEvent) {
                             alreadyAdded = true;
                             break;
                         }
                     }
                     if (!alreadyAdded) {
                         eventosParaRespostaCL.insereFinal(lastNonRGEvent);
                     }
                }
            }
        }
    }

    MergeSort<EventoBase*, ComparadorEventos>::sort(eventosParaRespostaCL, ComparadorEventos());
        
        std::cout << eventosParaRespostaCL.size() << std::endl;
        for (int i = 0; i < eventosParaRespostaCL.size(); ++i) {
            std::cout << eventosParaRespostaCL[i]->linhaOriginal << std::endl;
        }
    }

// Lógica de consulta PC 
void Logistica::processarConsultaPC(int dataHoraConsulta, int idPacote) {
    std::cout << std::setw(6) << std::setfill('0') << dataHoraConsulta << " PC " << std::setw(3) << std::setfill('0') << idPacote << std::endl;

    ListaEncadeada<std::string> respostaPC;

    Pacote* pacoteBuscado = indicePacotes.busca(idPacote);
    if (pacoteBuscado) {
        for (int i = 0; i < pacoteBuscado->historico.size(); ++i) {
            int eventIndex = pacoteBuscado->historico.getElemento(i);
            EventoBase* event = eventos[eventIndex];
            
            if (event->dataHora <= dataHoraConsulta) {
                respostaPC.adicionaNoFim(event->linhaOriginal);
            }
        }
    }
    std::cout << respostaPC.size() << std::endl;
    for (int i = 0; i < respostaPC.size(); ++i) {
        std::cout << respostaPC.getElemento(i) << std::endl;
    }
}