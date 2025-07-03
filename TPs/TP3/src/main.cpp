#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> // For std::setw and std::setfill
#include <fstream> // Necessário para std::ifstream

#include "../include/vetor.hpp"      // Your custom dynamic array
#include "../include/arvoreAVL.hpp"   // Your custom AVL tree
#include "../include/cliente.hpp"     // Cliente structure
#include "../include/pacotes.hpp"     // Pacote structure
#include "../include/lista.hpp"       // Your custom linked list
#include "../include/evento.hpp"      // Event structures and enum

// Forward declarations of helper functions
void processaEvento(const std::string& linha, VetorDinamico<EventoBase*>& eventos,
                    ArvoreAVL<std::string, Cliente>& indiceClientes,
                    ArvoreAVL<int, Pacote>& indicePacotes);

void processaConsulta(const std::string& linhaCompleta, int dataHoraConsulta,
                      const VetorDinamico<EventoBase*>& eventos,
                      ArvoreAVL<std::string, Cliente>& indiceClientes,
                      ArvoreAVL<int, Pacote>& indicePacotes);

int main(int argc, char* argv[]) {
    // Verifica se o número correto de argumentos foi fornecido
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << std::endl;
        return 1; // Retorna código de erro
    }

    std::string nomeArquivoEntrada = argv[1]; // O nome do arquivo é o primeiro argumento
    std::ifstream arquivoEntrada(nomeArquivoEntrada); // Tenta abrir o arquivo

    // Verifica se o arquivo foi aberto com sucesso
    if (!arquivoEntrada.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivoEntrada << std::endl;
        return 1; // Retorna código de erro
    }

    std::string linha;
    VetorDinamico<EventoBase*> eventos; // Central chronological storage of all event objects
    ArvoreAVL<std::string, Cliente> indiceClientes; // Client name -> Cliente object
    ArvoreAVL<int, Pacote> indicePacotes; // Package ID -> Pacote object

    // Lê a entrada linha por linha do arquivo
    while (std::getline(arquivoEntrada, linha)) { // Agora lendo de arquivoEntrada
        std::istringstream iss(linha);
        int dataHora;
        std::string tipoLinha; // "EV", "CL", "PC"

        // Tenta fazer o parsing dos dois primeiros tokens
        if (!(iss >> dataHora >> tipoLinha)) {
            // Lida com linha malformada ou vazia
            continue;
        }

        if (tipoLinha == "EV") {
            processaEvento(linha, eventos, indiceClientes, indicePacotes);
        } else if (tipoLinha == "CL" || tipoLinha == "PC") {
            // Processa a consulta imediatamente
            processaConsulta(linha, dataHora, eventos, indiceClientes, indicePacotes);
        }
    }

    // Limpa os objetos de evento alocados dinamicamente
    for (int i = 0; i < eventos.size(); ++i) {
        delete eventos[i];
    }

    arquivoEntrada.close(); // Fecha o arquivo de entrada
    return 0; // Saída bem-sucedida
}

// --- Helper Function Implementations ---

void processaEvento(const std::string& linha, VetorDinamico<EventoBase*>& eventos,
                    ArvoreAVL<std::string, Cliente>& indiceClientes,
                    ArvoreAVL<int, Pacote>& indicePacotes) {
    std::istringstream iss(linha);
    int dataHora, idPacote;
    std::string tipoLinha, tipoEventoStr;

    // Faz o parsing dos campos comuns
    iss >> dataHora >> tipoLinha >> tipoEventoStr >> idPacote;

    TipoEvento tipoEvento = stringToTipoEvento(tipoEventoStr);

    EventoBase* novoEvento = nullptr;

    // Faz o parsing dos campos específicos do evento e cria o objeto Evento correto
    if (tipoEvento == RG) {
        std::string remetente, destinatario;
        int armOrig, armDest;
        iss >> remetente >> destinatario >> armOrig >> armDest;
        novoEvento = new EventoRG(dataHora, idPacote, remetente, destinatario, armOrig, armDest, linha);
    } else if (tipoEvento == AR) {
        int armDest, secDest;
        iss >> armDest >> secDest;
        novoEvento = new EventoAR(dataHora, idPacote, armDest, secDest, linha);
    } else if (tipoEvento == RM) {
        int armOrig, secOrig; // Assumindo seção de origem para RM
        iss >> armOrig >> secOrig;
        novoEvento = new EventoRM(dataHora, idPacote, armOrig, secOrig, linha);
    } else if (tipoEvento == UR) {
        int armDest, secDest;
        iss >> armDest >> secDest;
        novoEvento = new EventoUR(dataHora, idPacote, armDest, secDest, linha);
    } else if (tipoEvento == TR) {
        int armOrig, armDest;
        iss >> armOrig >> armDest;
        novoEvento = new EventoTR(dataHora, idPacote, armOrig, armDest, linha);
    } else if (tipoEvento == EN) {
        int armDest; // Armazém de destino final
        iss >> armDest;
        novoEvento = new EventoEN(dataHora, idPacote, armDest, linha);
    } else {
        // Lida com tipo de evento inválido (ex: registra erro, pula)
        return;
    }

    if (novoEvento) {
        eventos.insereFinal(novoEvento); // Adiciona ao vetor cronológico central
        int eventIndex = eventos.size() - 1; // Índice do evento recém-adicionado

        // Atualiza o índice de Clientes
        // Para eventos RG, adiciona remetente e destinatario ao índice de clientes.
        if (tipoEvento == RG) {
            EventoRG* rgEvent = static_cast<EventoRG*>(novoEvento);
            
            // Lida com Remetente
            Cliente* remetenteCliente = indiceClientes.busca(rgEvent->remetente);
            if (!remetenteCliente) {
                Cliente novoCliente(rgEvent->remetente);
                novoCliente.pacotes.adicionaNoFim(idPacote); // Adiciona o ID do pacote à lista do cliente
                indiceClientes.inserir(rgEvent->remetente, novoCliente);
            } else {
                // Adiciona apenas se o ID do pacote ainda não estiver na lista do cliente
                if (!remetenteCliente->pacotes.contains(idPacote)) { // Requer o método 'contains' em ListaEncadeada
                    remetenteCliente->pacotes.adicionaNoFim(idPacote);
                }
            }
            
            // Lida com Destinatario
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

        // Atualiza o índice de Pacotes
        Pacote* pacoteAtual = indicePacotes.busca(idPacote);
        if (!pacoteAtual) {
            // Primeira vez que vê este pacote
            Pacote novoPacote(idPacote);
            novoPacote.linhaRG = eventIndex; // Índice do evento RG
            novoPacote.linhaUltimo = eventIndex;
            
            if (tipoEvento == RG) { // Define remetente/destinatario apenas a partir do evento RG
                EventoRG* rgEvent = static_cast<EventoRG*>(novoEvento);
                novoPacote.remetente = rgEvent->remetente;
                novoPacote.destinatario = rgEvent->destinatario;
            }
            novoPacote.historico.adicionaNoFim(eventIndex); // Adiciona o evento atual ao histórico
            indicePacotes.inserir(idPacote, novoPacote);
        } else {
            // Pacote já existe, apenas atualiza seu último evento conhecido e histórico
            pacoteAtual->linhaUltimo = eventIndex;
            pacoteAtual->historico.adicionaNoFim(eventIndex);
        }
    }
}


void processaConsulta(const std::string& linhaCompleta, int dataHoraConsulta,
                      const VetorDinamico<EventoBase*>& eventos,
                      ArvoreAVL<std::string, Cliente>& indiceClientes,
                      ArvoreAVL<int, Pacote>& indicePacotes) {
    std::istringstream iss(linhaCompleta);
    int dh;
    std::string tipoConsulta;
    iss >> dh >> tipoConsulta;

    if (tipoConsulta == "CL") {
        std::string nomeCliente;
        iss >> nomeCliente;
        // Imprime a linha da consulta formatada com 6 dígitos para a data/hora
        std::cout << std::setw(6) << std::setfill('0') << dh << " CL " << nomeCliente << std::endl;

        VetorDinamico<EventoBase*> eventosParaRespostaCL; // Armazena ponteiros para eventos relevantes

        Cliente* clienteBuscado = indiceClientes.busca(nomeCliente);
        if (clienteBuscado) {
            // Itera sobre os pacotes associados a este cliente
            for (int i = 0; i < clienteBuscado->pacotes.size(); ++i) {
                int idPacote = clienteBuscado->pacotes.getElemento(i);
                Pacote* pacoteInfo = indicePacotes.busca(idPacote);
                
                if (pacoteInfo) {
                    // 1. Adiciona o evento RG se o cliente é remetente ou destinatário e dentro do tempo da consulta
                    EventoRG* rgEvent = static_cast<EventoRG*>(eventos[pacoteInfo->linhaRG]);
                    if (rgEvent && rgEvent->dataHora <= dataHoraConsulta) {
                        if (rgEvent->remetente == nomeCliente || rgEvent->destinatario == nomeCliente) {
                            // Verifica se o evento já foi adicionado para evitar duplicatas
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

                    // 2. Encontra o ÚLTIMO evento relevante para este pacote (que não seja RG)
                    //    Isso pode ser AR, RM, UR, TR, EN.
                    EventoBase* lastNonRGEvent = nullptr;
                    for (int j = 0; j < pacoteInfo->historico.size(); ++j) {
                        int eventIndex = pacoteInfo->historico.getElemento(j);
                        EventoBase* currentEvent = eventos[eventIndex];
                        
                        if (currentEvent->dataHora <= dataHoraConsulta) {
                            if (currentEvent->tipo != RG) { // Considera apenas eventos que não são RG
                                if (!lastNonRGEvent || currentEvent->dataHora > lastNonRGEvent->dataHora) {
                                    lastNonRGEvent = currentEvent;
                                } else if (currentEvent->dataHora == lastNonRGEvent->dataHora) {
                                    // Se dataHora for a mesma, e currentEvent for diferente de lastNonRGEvent,
                                    // currentEvent é o mais recente na ordem do histórico para o mesmo timestamp.
                                    // Isso corrige o problema do RM vs TR: o TR aparece depois no histórico para o mesmo tempo.
                                    lastNonRGEvent = currentEvent; 
                                }
                            }
                        }
                    }
                    if (lastNonRGEvent) {
                         // Verifica se este evento já foi adicionado
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

        // Ordena os eventos coletados por dataHora, depois por idPacote para desempate
        // Usando Selection Sort para VetorDinamico (como estruturas da STL não são permitidas)
        for (int i = 0; i < eventosParaRespostaCL.size(); ++i) {
            int min_idx = i;
            for (int j = i + 1; j < eventosParaRespostaCL.size(); ++j) {
                // Critério de ordenação principal: dataHora
                if (eventosParaRespostaCL[j]->dataHora < eventosParaRespostaCL[min_idx]->dataHora) {
                    min_idx = j;
                } 
                // Critério de desempate: idPacote
                else if (eventosParaRespostaCL[j]->dataHora == eventosParaRespostaCL[min_idx]->dataHora) {
                    // Prioriza o evento com o ID do pacote menor para desempate
                    if (eventosParaRespostaCL[j]->idPacote < eventosParaRespostaCL[min_idx]->idPacote) {
                        min_idx = j;
                    }
                }
            }
            if (min_idx != i) {
                // Troca os elementos para ordenar
                EventoBase* temp = eventosParaRespostaCL[i];
                eventosParaRespostaCL[i] = eventosParaRespostaCL[min_idx];
                eventosParaRespostaCL[min_idx] = temp;
            }
        }
        
        std::cout << eventosParaRespostaCL.size() << std::endl; // Imprime o número de linhas de resposta
        for (int i = 0; i < eventosParaRespostaCL.size(); ++i) {
            std::cout << eventosParaRespostaCL[i]->linhaOriginal << std::endl; // Imprime cada linha original
        }

    } else if (tipoConsulta == "PC") {
        int idPacote;
        iss >> idPacote;
        // Imprime a linha da consulta formatada com 6 dígitos para a data/hora
        std::cout << std::setw(6) << std::setfill('0') << dh << " PC " << std::setw(3) << std::setfill('0') << idPacote << std::endl;

        ListaEncadeada<std::string> respostaPC; // Armazena as linhas originais dos eventos para consulta PC

        Pacote* pacoteBuscado = indicePacotes.busca(idPacote);
        if (pacoteBuscado) {
            // Itera sobre os eventos no histórico do pacote
            for (int i = 0; i < pacoteBuscado->historico.size(); ++i) {
                int eventIndex = pacoteBuscado->historico.getElemento(i);
                EventoBase* event = eventos[eventIndex];
                
                // Inclui apenas eventos até o timestamp da consulta
                if (event->dataHora <= dataHoraConsulta) {
                    respostaPC.adicionaNoFim(event->linhaOriginal); // Adiciona a linha original exata
                }
            }
        }
        std::cout << respostaPC.size() << std::endl; // Imprime o número de linhas de resposta
        for (int i = 0; i < respostaPC.size(); ++i) {
            std::cout << respostaPC.getElemento(i) << std::endl; // Imprime cada linha original
        }
    }
}