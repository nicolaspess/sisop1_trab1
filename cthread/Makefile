#
# Makefile de EXEMPLO
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# É NECESSARIO ADAPTAR ESSE ARQUIVO de makefile para suas necessidades.
#  1. Cuidado com a regra "clean" para não apagar o "support.o"
#
# OBSERVAR que as variáveis de ambiente consideram que o Makefile está no diretótio "cthread"
# 

CC=gcc
LIB_DIR=./lib/
INC_DIR=./include/
BIN_DIR=./bin/
SRC_DIR=./src/

all: libcthread.a

libcthread.a: cdata.o cthread.o
	ar crs libcthread.a $(BIN_DIR)cthread.o $(BIN_DIR)support.o $(BIN_DIR)cdata.o
	mv libcthread.a $(LIB_DIR)

cdata.o: 
	$(CC) -g -c $(SRC_DIR)cdata.c -Iinclude -Wall
	mv ./cdata.o $(BIN_DIR)

cthread.o:
	$(CC) -g -c $(SRC_DIR)cthread.c -Iinclude -Wall
	mv ./cthread.o $(BIN_DIR)	

clean:
	rm -rf $(LIB_DIR)/*.a $(SRC_DIR)/*~ $(BIN_DIR)/cdata.o $(BIN_DIR)/cthread.o *~


