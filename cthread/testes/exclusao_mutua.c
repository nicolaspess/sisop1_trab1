#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>

csem_t mutex;

void* func0(void *arg){
	cwait(&mutex);
	printf("eu sou a func0 fazendo cwait e cedecendo a cpu... \n");
	cyield();
	printf("func0 apos ceder a cpu\n");
	csignal(&mutex);
	printf("func0 apos liberar recurso\n");
	return NULL;
}

void* func1(void *arg){
	printf("eu sou a func1 solicitando recursobloqueado e sendo bloqueada.. \n");
	cwait(&mutex);
	printf("sou a func1 apos solicitar o recurso do mutex\n");
	return NULL;
}

int main(int argc, char *argv[]){
	int id0;
	int i = 999;
	printf("teste de exclusão mutua (mutex)...\n");
	csem_init(&mutex,1);
	id0 = ccreate(func0, (void *)&i,0);
	ccreate(func1, (void *)&i,0);
	cjoin(id0);
	printf("thread main encerrando o programa... \n");
	return 0;
}