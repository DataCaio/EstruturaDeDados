#!/bin/bash

# --- Configurações dos Experimentos ---
# Adicione ou remova valores destas listas para customizar os testes.
#
# NUM_PACOTES: Número de eventos de registro de pacotes a serem gerados.
# NUM_CLIENTES: Número total de clientes únicos (remetentes/destinatários).
# NUM_QUERIES: Número total de consultas (metade será CL, metade PC).
# SEED: Semente para o gerador de números aleatórios, para reprodutibilidade.

declare -a ARR_NUM_PACOTES=(50 100 150 200 250 300 350 400 450 500 550 600 650 700 750 800 850 900 950)
declare -a ARR_NUM_CLIENTES=(50 100 150 200 250 300 350 400 450 500 550 600 650 700 750 800 850 900 950)
declare -a ARR_NUM_QUERIES=(50 100 150 200 250 300 350 400 450 500 550 600 650 700 750 800 850 900 950)
SEED=42

# --- Compilação e Execução ---

echo "Compilando os projetos..."
# Compila o gerador de workload
(cd experiments && gcc -o genwkl3 genwkl3.c -lm)
# Compila o programa principal de logística
make all
if [ $? -ne 0 ]; then
    echo "Erro na compilação. Abortando."
    exit 1
fi

# Cria diretório para salvar os resultados, se não existir
mkdir -p tempos_resultados

echo "Iniciando a execução dos experimentos..."

# Loop através de todas as combinações de parâmetros
for pacotes in "${ARR_NUM_PACOTES[@]}"; do
  for clientes in "${ARR_NUM_CLIENTES[@]}"; do
    for queries in "${ARR_NUM_QUERIES[@]}"; do
      
      # Calcula o número de cada tipo de query (metade/metade)
      queries_cl=$((queries / 2))
      queries_pc=$((queries / 2))

      echo "--------------------------------------------------------------------"
      echo "Executando com: Pacotes=${pacotes}, Clientes=${clientes}, Queries=${queries}"
      echo "--------------------------------------------------------------------"

      # Nome do arquivo de tempo, seguindo o padrão do notebook de análise
      TIMING_FILE="tempos_p${pacotes}_c${clientes}_q${queries}.txt"

      # 1. Gera os arquivos de workload (tp2.wkl e tp3.wkl)
      (cd experiments && ./genwkl3 "$pacotes" "$clientes" "$queries_cl" "$queries_pc" "$SEED")
      
      # 2. Simula o passo do TP2 (assumindo que 'sched', 'tp2tp3.sed' e 'intp3.awk' estão na pasta 'experiments')
      (cd experiments && ./sched < tp2.wkl > sched.log)
      (cd experiments && sed -f tp2tp3.sed sched.log | awk -f intp3.awk > tmptp3.out)
      
      # 3. Combina, ordena e cria o arquivo de entrada final para o tp3
      (cd experiments && cat tp3.wkl tmptp3.out | sort > ../tp3_final.in)

      # 4. Executa o programa principal e redireciona o stderr (tempos) para o arquivo
      ./bin/tp3.out tp3_final.in 2> "tempos_resultados/${TIMING_FILE}"

      echo "Experimento concluído. Resultados salvos em: tempos_resultados/${TIMING_FILE}"
    done
  done
done

echo "Todos os experimentos foram concluídos!"

# Opcional: Limpeza dos arquivos gerados
# make clean
# rm -f experiments/{genwkl3,tp2.wkl,tp3.wkl,sched.log,tmptp3.out} tp3_final.in