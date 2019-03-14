CC = g++
SRC = main.cpp 
OBJ = math

all:
	$(CC) $(SRC) -o $(OBJ) -std=c++11
