#include "../include/logistica.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>

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
    int tempo, idPacote;
    std::string tipoLinha, tipoEventoStr;

    iss >> tempo >> tipoLinha >> tipoEventoStr >> idPacote;

    TipoEvento tipoEvento = stringToTipoEvento(tipoEventoStr);
    
    // Movendo a lógica de criação de eventos para cá
    if (tipoEvento == RG) {
        std::string remetente, destinatario;
        int armOrig, armDest;
        iss >> remetente >> destinatario >> armOrig >> armDest;
        return new EventoRG(tempo, idPacote, remetente, destinatario, armOrig, armDest, linha);
    } else if (tipoEvento == AR) {
        int armDest, secDest;
        iss >> armDest >> secDest;
        return new EventoAR(tempo, idPacote, armDest, secDest, linha);
    } else if (tipoEvento == RM) {
        int armOrig, secOrig;
        iss >> armOrig >> secOrig;
        return new EventoRM(tempo, idPacote, armOrig, secOrig, linha);
    } else if (tipoEvento == UR) {
        int armDest, secDest;
        iss >> armDest >> secDest;
        return new EventoUR(tempo, idPacote, armDest, secDest, linha);
    } else if (tipoEvento == TR) {
        int armOrig, armDest;
        iss >> armOrig >> armDest;
        return new EventoTR(tempo, idPacote, armOrig, armDest, linha);
    } else if (tipoEvento == EN) {
        int armDest;
        iss >> armDest;
        return new EventoEN(tempo, idPacote, armDest, linha);
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
    int tempo;
    std::string tipoLinha;

    if (!(iss >> tempo >> tipoLinha)) {
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
        processarConsultaCL(tempo, nomeCliente);
    } else if (tipoLinha == "PC") {
        int idPacote;
        iss >> idPacote;
        processarConsultaPC(tempo, idPacote);
    }
}

// Lógica de consulta CL 
void Logistica::processarConsultaCL(int tempoConsulta, const std::string& nomeCliente) {
    std::cout << std::setw(6) << std::setfill('0') << tempoConsulta << " CL " << nomeCliente << std::endl;

    VetorDinamico<EventoBase*> eventosParaRespostaCL;

    Cliente* clienteBuscado = indiceClientes.busca(nomeCliente);
    if (clienteBuscado) {
        No<int>* noPacote = clienteBuscado->pacotes.getCabeca();
            while (noPacote != nullptr) {
                int idPacote = noPacote->dado;
                Pacote* pacoteInfo = indicePacotes.busca(idPacote);
                
                if (pacoteInfo) {
                    // Adiciona o evento de Registro (RG) se for válido
                    EventoRG* rgEvent = static_cast<EventoRG*>(eventos[pacoteInfo->linhaRG]);
                    if (rgEvent && rgEvent->tempo <= tempoConsulta) {
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

                    // Encontra o último evento não-RG antes do tempo da consulta
                    EventoBase* lastNonRGEvent = nullptr;
                    
                    No<int>* noHistorico = pacoteInfo->historico.getCabeca();
                    while (noHistorico != nullptr) {
                        int eventIndex = noHistorico->dado;
                        EventoBase* currentEvent = eventos[eventIndex];
                        
                        if (currentEvent->tempo <= tempoConsulta) {
                            if (currentEvent->tipo != RG) {
                                if (!lastNonRGEvent || currentEvent->tempo >= lastNonRGEvent->tempo) {
                                    lastNonRGEvent = currentEvent;
                                }
                            }
                        }
                        noHistorico = noHistorico->proximo;
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
                noPacote = noPacote->proximo;
            }
        }

    for (int i = 0; i < eventosParaRespostaCL.size(); ++i) {
        int min_idx = i;
        for (int j = i + 1; j < eventosParaRespostaCL.size(); ++j) {
            if (eventosParaRespostaCL[j]->tempo < eventosParaRespostaCL[min_idx]->tempo) {
                min_idx = j;
            } 
            else if (eventosParaRespostaCL[j]->tempo == eventosParaRespostaCL[min_idx]->tempo) {
                if (eventosParaRespostaCL[j]->idPacote < eventosParaRespostaCL[min_idx]->idPacote) {
                    min_idx = j;
                }
            }
        }
        if (min_idx != i) {
            EventoBase* temp = eventosParaRespostaCL[i];
            eventosParaRespostaCL[i] = eventosParaRespostaCL[min_idx];
            eventosParaRespostaCL[min_idx] = temp;
        }
    }
    
    std::cout << eventosParaRespostaCL.size() << std::endl;
    for (int i = 0; i < eventosParaRespostaCL.size(); ++i) {
        std::cout << eventosParaRespostaCL[i]->linhaOriginal << std::endl;
    }
}

// Lógica de consulta PC 
void Logistica::processarConsultaPC(int tempoConsulta, int idPacote) {
    std::cout << std::setw(6) << std::setfill('0') << tempoConsulta << " PC " << std::setw(3) << std::setfill('0') << idPacote << std::endl;

    ListaEncadeada<std::string> respostaPC;

    Pacote* pacoteBuscado = indicePacotes.busca(idPacote);
    if (pacoteBuscado) {
        No<int>* noAtual = pacoteBuscado->historico.getCabeca();
        while (noAtual != nullptr) {
            int eventIndex = noAtual->dado;
            EventoBase* event = eventos[eventIndex];
            if (event->tempo <= tempoConsulta) {
                respostaPC.adicionaNoFim(event->linhaOriginal);
            }
            noAtual = noAtual->proximo;
        }
    }
    std::cout << respostaPC.size() << std::endl;
    for (int i = 0; i < respostaPC.size(); ++i) {
        std::cout << respostaPC.getElemento(i) << std::endl;
    }
}