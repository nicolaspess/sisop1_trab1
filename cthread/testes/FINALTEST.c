#include <stdio.h>
#include <stdlib.h>
#include "../include/support.h"
#include "../include/cthread.h"
#define MAX 10
int i=0;
int id[MAX];
csem_t rec_imaginario;
int indice_de_procrastinacao=3;

void* acabador(void *arg){
	printf("acabador:: Ja posso acabar?\n");
	cyield();
	cwait(&rec_imaginario);
	printf("acabador:: Finalmente, acabando o programa!\n\n\n");
	csignal(&rec_imaginario);

	return NULL;
}

void* procrastinador(void *arg){
	int cont;

	for(cont=0;cont<indice_de_procrastinacao; cont++){
		printf("procrastinador:: ainda nao!\n");
		cyield();
	}
	return NULL;

}

void* mestre(void *arg){
	printf("mestre:: Bloqueando o hardware de acabar programas!\n");
	cwait(&rec_imaginario);
	cyield();
	printf("mestre:: Mudando a prioridade do acabador!\n");
	csetprio(id[MAX-2],3);
	printf("mestre:: Liberando o hardware de acabar programas!\n");
	csignal(&rec_imaginario);

	return NULL;
}


int main(int argc, char *argv[]){
	int cont;
	
	if (argc<=1){
		indice_de_procrastinacao = 2;
	}
	else
		indice_de_procrastinacao = atoi(argv[1])+2;

	printf("\n\n\nIniciando um programa que quer acabar!\n");
	
	csem_init(&rec_imaginario,1);
	id[MAX-1] = ccreate(mestre, (void *)&i,0);
	id[MAX-2] = ccreate(acabador, (void *)&i,0);

	for(cont=0; cont<MAX-2;cont++)
		id[cont] = ccreate(procrastinador, (void *)&i,0);
	
	for(cont=0; cont<MAX;cont++)
		cjoin(id[cont]);

	

	printf("\n\n");
	return 0;
}