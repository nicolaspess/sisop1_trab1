#
# Makefile
#

CC=gcc
LIB_DIR=../lib

all: foo_bar exclusao_mutua

foo_bar: foo_bar.c $(LIB_DIR)/libcthread.a
	$(CC) -o foo_bar foo_bar.c -L$(LIB_DIR) -lcthread -Wall

exclusao_mutua: exclusao_mutua.c $(LIB_DIR)/libcthread.a
	$(CC) -o exclusao_mutua exclusao_mutua.c -L$(LIB_DIR) -lcthread -Wall

clean:
	rm -rf foo_bar exclusao_mutua *~