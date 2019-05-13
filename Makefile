CC = g++
SRC = main.cpp 
OBJ = math
INCLUDE_DIR = include

all:
	@$(CC) $(SRC)-I$(INCLUDE_DIR)  -o $(OBJ) -std=c++11
clean:
	@rm -rf ./math
