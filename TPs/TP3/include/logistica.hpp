// TP3/include/logistica.hpp
#ifndef SISTEMA_LOGISTICO_HPP
#define SISTEMA_LOGISTICO_HPP

#include <string>
#include <vector> // Adicionado para std::vector (embora não mais diretamente no public para o main)
#include "vetor.hpp"
#include "arvoreAVL.hpp"
#include "cliente.hpp"
#include "pacotes.hpp"
#include "evento.hpp"
#include "lista.hpp" 

class Logistica {
private:
    VetorDinamico<EventoBase*> eventos; // Armazenamento cronológico central
    ArvoreAVL<std::string, Cliente> indiceClientes; // Índice de clientes
    ArvoreAVL<int, Pacote> indicePacotes; // Índice de pacotes

    // Variáveis para acumular os tempos de execução em microssegundos (us)
    long long totalEventProcessingTimeUs;
    long long totalCLQueryTimeUs;
    long long totalPCQueryTimeUs;

    // Métodos privados para modularizar processaEvento e processaConsulta
    EventoBase* criarEventoDeLinha(const std::string& linha);
    void atualizarIndices(EventoBase* novoEvento, int eventIndex);
    void processarConsultaCL(int tempoConsulta, const std::string& nomeCliente);
    void processarConsultaPC(int tempoConsulta, int idPacote);

public:
    Logistica();
    ~Logistica();

    void processarLinha(const std::string& linha); // Método público para main

    // Métodos públicos para obter os tempos acumulados
    long long getTotalEventProcessingTime() const;
    long long getTotalCLQueryTime() const;
    long long getTotalPCQueryTime() const;
};

#endif