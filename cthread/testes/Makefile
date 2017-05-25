#
# Makefile
#

CC=gcc
LIB_DIR=../lib

all: foo_bar testa_semaforo foo_bar_testacsetprio FINALTEST

foo_bar: foo_bar.c $(LIB_DIR)/libcthread.a
	$(CC) -o foo_bar foo_bar.c -L$(LIB_DIR) -lcthread -Wall

foo_bar_testacsetprio: foo_bar_testacsetprio.c $(LIB_DIR)/libcthread.a
	$(CC) -o foo_bar_testacsetprio foo_bar_testacsetprio.c -L$(LIB_DIR) -lcthread -Wall

testa_semaforo: testa_semaforo.c $(LIB_DIR)/libcthread.a
	$(CC) -o testa_semaforo testa_semaforo.c -L$(LIB_DIR) -lcthread -Wall

FINALTEST: FINALTEST.c $(LIB_DIR)/libcthread.a
	$(CC) -o FINALTEST FINALTEST.c -L$(LIB_DIR) -lcthread -Wall

clean:
	rm -rf foo_bar foo_bar_testacsetprio testa_semaforo FINALTEST *~
