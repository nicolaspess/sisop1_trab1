#include "../include/support.h"
#include "../include/cdata.h"
#include "../include/cthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

void init(){
	if(FORWARDER == NULL){
		ucontext *main_ctx, *forwarder_ctx;
		TCB_t *mainTCB;

		CreateFila2(&aptos0);
		CreateFila2(&aptos1);
		CreateFila2(&aptos2);
		CreateFila2(&aptos3);
		CreateFila2(&bloqueados);

		forwarder_ctx = (ucontext_t *)malloc(sizeof(ucontext_t));
		forwarder_ctx->uc_stack.ss_sp = malloc(SIGSTKSZ);
		forwarder_ctx->uc_stack.ss_size = SIGSTKSZ;
		forwarder_ctx->uc_link = 0;
		getcontext(forwarder_ctx);
		makecontext(forwarder_ctx, (void (*)(void))forwarder,0);
		FORWARDER = forwarder_ctx;

		main_ctx = (ucontext_t *)malloc(sizeof(ucontext_t));
		main_ctx->uc_stack.ss_sp = malloc(SIGSTKSZ);
		main_ctx->uc_stack.ss_size = SIGSTKSZ;
		main_ctx->uc_link = 0;

		mainTCB = (TCB_t *)malloc(sizeof(TCB_t));
		mainTCB->tid= 0;
		mainTCB->state=2;//executando
		mainTCB->context=*main_ctx;
		EXECUTANDO = mainTCB;
		getcontext(&mainTCB->context);
	}
}

//equivale ao dispatcher
void forwarder(){
	TCB_t *prox, *exe;
	exe = EXECUTANDO;//thread q estava executando
	//exe->state = 4;//termino
	free(exe);

	prox = escalonador(); //primeiro da lista de aptos de acordo com prioridade
	removeDeApto(prox->tid, prox->ticket);
	EXECUTANDO = prox;

	setcontext(&prox->context);
}

//seleciona a próxima thread a ser executada de acordo com a prioridade
TCB_t escalonador(){
	int n;
	TCB_t *tcb;
	tcb = NULL;
	if(FirstFila2(&aptos0) == 0){
		//fila não vazia
		tcb = (TCB_t *)GetAtIteratorFila2(&aptos0);
	}else if(FirstFila2(&aptos1) == 0){
		tcb = (TCB_t *)GetAtIteratorFila2(&aptos1);
	}else if(FirstFila2(&aptos2) == 0){
		tcb = (TCB_t *)GetAtIteratorFila2(&aptos2);
	}else if(FirstFila2(&aptos3) == 0){
		tcb = (TCB_t *)GetAtIteratorFila2(&aptos3);
	}
	return tcb;
}

//procura e remove da lista de apto da sua prioridade
void removeDeApto(int id, int prioridade){
	TCB_t *tcb;
	switch (prioridade){
		case 0:
			FirstFila2(&aptos0);
			while((tcb = GetAtIteratorFila2(&aptos0))!=NULL){
				if(tcb->tid == id){
					DeleteAtIteratorFila2(&aptos0);
					break;
				}
				NextFila2(&aptos0);
			}
			break;
		case 1:
			FirstFila2(&aptos1);
			while((tcb = GetAtIteratorFila2(&aptos1))!=NULL){
				if(tcb->tid == id){
					DeleteAtIteratorFila2(&aptos1);
					break;
				}
				NextFila2(&aptos1);
			}
			break;
		case 2:
			FirstFila2(&aptos2);
			while((tcb = GetAtIteratorFila2(&aptos2))!=NULL){
				if(tcb->tid == id){
					DeleteAtIteratorFila2(&aptos2);
					break;
				}
				NextFila2(&aptos2);
			}
			break;
		case 3:
			FirstFila2(&aptos3);
			while((tcb = GetAtIteratorFila2(&aptos3))!=NULL){
				if(tcb->tid == id){
					DeleteAtIteratorFila2(&aptos3);
					break;
				}
				NextFila2(&aptos3);
			}
			break;
	}
}

//procura e remove tcb de tid=id
void removeDeBloqueado(int id){
TCB_t *tcb;
	FirstFila2(&bloqueados);
	while((tcb = GetAtIteratorFila2(&bloqueados))!=NULL){
		if(tcb->tid == id){
			DeleteAtIteratorFila2(&bloqueados);
			break;
		}
		NextFila2(&bloqueados);
	}
}