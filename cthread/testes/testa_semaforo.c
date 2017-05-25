#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>

csem_t rec_imaginario;

void* func0(void *arg){
	printf("func0:: pedindo recurso imaginario\n");
	cwait(&rec_imaginario);
	printf("func0:: usando recurso imaginario \n");
	cyield();
	csignal(&rec_imaginario);
	printf("func0:: liberando recurso imaginario\n");
	return NULL;
}

void* func1(void *arg){
	printf("func1:: pedindo recurso imaginario \n");
	cwait(&rec_imaginario);
	printf("func1:: usando recurso imaginario\n");
	cyield();
	csignal(&rec_imaginario);
	printf("func1:: liberando recurso imaginario\n");
	return NULL;
}

int main(int argc, char *argv[]){
	int id0,id1;
	int i=0;
	printf("\n\nInicio do testa_semaforo\n");
	

	csem_init(&rec_imaginario,1);
	id0 = ccreate(func0, (void *)&i,0);
	id1 = ccreate(func1, (void *)&i,0);
	
	cjoin(id0);cjoin(id1);


	printf("Fim testa_semaforo\n\n\n");
	return 0;
}