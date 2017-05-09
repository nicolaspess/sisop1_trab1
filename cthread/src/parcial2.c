#include "../include/support.h"
#include "../include/cdata.h"
#include "../include/cthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

int ids = 0;
void createContext(PFILA2 aptos, void* (*start)(void*)){
	
	ucontext_t *newContext;

	newContext = (ucontext_t *)(malloc(sizeof(ucontext_t)));
	newContext->uc_stack.ss_sp = malloc(SIGSTKSZ);
	newContext->uc_stack.ss_size = SIGSTKSZ;
	newContext->uc_link = NULL;

	TCB_t *newTCB;
	newTCB = (TCB_t *)(malloc(sizeof(TCB_t)));
	newTCB->tid = ids++;
	newTCB->state = 1; //estado apto
	newTCB->context = *newContext;


	AppendFila2(&aptos, (void *)newTCB); //insere no fim da fila
	getcontext(&newTCB->context);
	makecontext(&newTCB->context, (void (*)(void))start, 0);//makecontext(thread->context, funçao associada, nroArgumentos, argumentos)
}


void progAritmetica(){
	//8 termos
}

void progGeometrica(){
	//10 termos
}

void fibonacci(){
	//12 termos
}

void defNumerosTriangulares(){
	//6 nros
}

int main(){

	//ponteiro das filas
	FILA2 aptos;
	FILA2 executando;
	FILA2 bloqueados;
	
	//criando as filas
	if (CreateFila2(&aptos) != 0) printf("erro ao criar fila aptos \n");
	if (CreateFila2(&executando) != 0) printf("erro ao criar fila executando \n");
	if (CreateFila2(&bloqueados) != 0) printf("erro ao criar fila bloqueados \n");
	//criadas filas vazias

	//criando os contextos e colocando-os na ordem na fila de aptos(acho q faz isso)
	createContext(&aptos, progAritmetica);
	createContext(&aptos, progGeometrica);
	createContext(&aptos, fibonacci);
	createContext(&aptos, defNumerosTriangulares);
}