# Compilador
CXX = g++
# Flags de compilação
CXXFLAGS = -Wall -std=c++11 -g -I$(INC_DIR)
# Flags para depuração (descomente se necessário)
# CXXFLAGS += -g

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

# Nome do executável
TARGET = $(BIN_DIR)/tp2.out

# Arquivos fonte
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Arquivos objeto
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Dependências (arquivos de cabeçalho)
DEPS = $(wildcard $(INC_DIR)/*.hpp)

# Regra padrão
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para compilar arquivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para limpar
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

# Regra para rodar
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean ru