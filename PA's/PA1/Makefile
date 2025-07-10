CC = g++                  
CFLAGS = -Wall -Iinclude  
SRC = src/main.cpp         
OBJ = obj/main.o           
EXEC = bin/pa1.out         

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f obj/*.o bin/pa1.out

