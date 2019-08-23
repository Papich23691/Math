CC = g++
SRC = main.cpp 
OBJ = math
INCLUDE_DIR = include

all:
	@$(CC) $(SRC)-I$(INCLUDE_DIR) -o $(OBJ)  -std=c++14 -Wnull-conversion 
clean:
	@rm -rf ./math
