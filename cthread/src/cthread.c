#include "../include/support.h"
#include "../include/cdata.h"
#include "../include/cthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

int ids = 0;

TCB_t *EXECUTANDO = NULL;
ucontext_t *FORWARDER = NULL;

//cria a thread
int ccreate (void* (*start)(void*), void *arg, int prio){
	TCB_t *newTCB
	ucontext_t *newContext

	init(); //seta todas as diretrizes de fila

	newTCB = (TCB_t *)malloc(sizeof(TCB_t));
	newTCB = (ucontext_t *)malloc(sizeof(ucontext_t));

	if(newTCB == NULL){
		return -1;
	}

	newContext->uc_stack.ss_sp = malloc(SIGSTKSZ);
	newContext->uc_stack.ss_size = SIGSTKSZ;
	newContext->uc_link = FORWARDER;

	newTCB->ticket = prio;
	newTCB->state = 1;//apto
	newTCB->tid = ids++;
	newTCB->context = *newContext;

	//adiciona na fila de aptos
	switch (prio){
		case 0:
			AppendFila2(&aptos0, (void*)newTCB);
			break;
		case 1:
			AppendFila2(&aptos1, (void*)newTCB);
			break;
		case 2:
			AppendFila2(&aptos2, (void*)newTCB);
			break;
		case 3:
			AppendFila2(&aptos3, (void*)newTCB);
			break;
	}
	
	AppendFila2(&aptos, (void*)newTCB);
	getcontext(&newTCB->context);
	makecontext(&newTCB->context, (void (*)(void))start, 1, arg);

	return newTCB->tid;
}
//troca a prioridade
int csetprio(int tid, int prio){

}

//libera o processador
int cyield(void){
	TCB_t *yielder, *prox;

	//poe o q tava executando no fim da fila
	yielder = EXECUTANDO;
	yielder->state=1;
	switch (yielder->ticket){
		case 0:
			AppendFila2(&aptos0, (void*)yielder);
			break;
		case 1:
			AppendFila2(&aptos1, (void*)yielder);
			break;
		case 2:
			AppendFila2(&aptos2, (void*)yielder);
			break;
		case 3:
			AppendFila2(&aptos3, (void*)yielder);
			break;
	}

	prox = escalonador();
	if(prox == NULL){
		return -1;
	}
	prox->state = 2;
	EXECUTANDO = prox;
	
	swapcontext(&yielder->context, &prox->context);

	return 0;
}

int cjoin(int tid){
	TCB_t *escolhido, *bloqueado;
	//BLOCKED_t *nodoBloqueado;

	init();

	escolhido = escalonador(); //escolhe o de maior prioridade pra assumir
	if(escolhido == NULL){
		return -1;
	}
	bloqueado = EXECUTANDO;
	bloqueado->state = 3;//bloqueado

	//nodoBloqueado->id = tid;
	//nodoBloqueado->tcb = bloqueado;

	AppendFila2(&bloqueados, (void*)bloqueado);
	removeDeApto(escolhido->tid, escolhido->ticket);
	escolhido->state = 2;
	EXECUTANDO = escolhido;

	swapcontext(&bloqueado->context, &escolhido->context);

	return 0;

}

int csem_init(csem_t *sem, int count){
	FILA2 *fila;

	init();

	if(sem == NULL){
		return -1;
	}
	fila = (FILA2*) malloc(sizeof(FILA2));
	if(fila == NULL){
		return -1;
	}
	CreateFila2(fila);
	sem->count = count;
	sem->fila = fila;
}

int cwait(csem_t *sem){
	TCB_t *escolhido, *bloqueado;
	//BLOCKED_t *nodoBloqueado;

	init();

	if(sem == NULL) return -1;

	sem->count = sem->count - 1;

	if(sem->count < 0){
		escolhido = escalonador();
		if(escolhido == NULL){
			return -1;
		}

		//nodoBloqueado = (BLOCKED_t *)malloc(sizeof(BLOCKED_t));
		//if(nodoBloqueado == NULL) return -1;

		bloqueado = EXECUTANDO;
		bloqueado->state = 3;//bloqueado

		//add na fila
		AppendFila2(&bloqueados, (void*)bloqueado);
		AppendFila2(sem->fila, bloqueado);

		removeDeApto(escolhido->tid, escolhido->ticket);
		escolhido->state = 2;//executando
		EXECUTANDO = escolhido;

		swapcontext(&bloqueado->context, &escolhido->context);
	}
	return 0;
}
int csignal(csem_t *sem){
	TCB_t *bloqueado;

	init();
	if(sem == NULL){
		return -1;
	}
	sem->count += 1;
	FirstFila2(sem->fila);
	bloqueado = GetAtIteratorFila2(sem->fila);

	if(bloqueado){
		DeleteAtIteratorFila2(sem->fila);
		bloqueado->state = 1;//apto
		removeDeBloqueado(bloqueado->tid);
		switch (bloqueado->ticket){
			case 0:
				AppendFila2(&aptos0, (void*)bloqueado);
				break;
			case 1:
				AppendFila2(&aptos1, (void*)bloqueado);
				break;
			case 2:
				AppendFila2(&aptos2, (void*)bloqueado);
				break;
			case 3:
				AppendFila2(&aptos3, (void*)bloqueado);
				break;
		}
	}
	return 0;
}
int cidentify (char *name, int size){
	char *nomes = (char *)malloc(sizeof(char *));
	if(name==NULL){
		return -1;
	}
	strcpy(name, "João Batista Manique Henz matricula e Nícolas Vincent Dall Bello Pessutto 242284");
	name=nomes;
	return 0;
}