#include "../include/support.h"
#include "../include/cdata.h"
#include "../include/cthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

int ids = 0;
FILA2 aptos;
TCB_t *EXECUTANDO;

ucontext_t forwarder_ctx, main_ctx, arit_ctx, geo_ctx, fibo_ctx, tri_ctx;

//qnd uma thread termina essa função encaminha para a próxima dos aptos
void forwarder(){
	TCB_t *prox, *exe;
	exe = EXECUTANDO;//thread q estava executando
	//exe->state = 4;//termino
	free(exe);

	FirstFila2(&aptos);
	prox = (TCB_t *)GetAtIteratorFila2(&aptos); //primeiro da lista de aptos
	DeleteAtIteratorFila2(&aptos);
	EXECUTANDO = prox;

	setcontext(&prox->context);
}

void yield(){
	TCB_t *yielder, *first;

	//poe o q tava executando no fim da fila
	yielder = EXECUTANDO;
	yielder->state=1;
	AppendFila2(&aptos, (void *)yielder);

	FirstFila2(&aptos);
	first = (TCB_t *)GetAtIteratorFila2(&aptos);
	DeleteAtIteratorFila2(&aptos);
	first->state=2;
	EXECUTANDO = first;

	
	swapcontext(&yielder->context, &first->context);
}

void progAritmetica(void){
	int cont;
	int termoAtual=0;
	int repeticoes=8;//8
	int inicio=1;
	int r=4;

	for (cont=1;cont<=repeticoes;cont++){

		termoAtual = termoAtual + r;

		if (cont==1){
			termoAtual = inicio;
		}

		printf("PA   termo %2d : %4d \n",cont,termoAtual);
		yield();
	}

}

void progGeometrica(void){
	int cont;
	int termoAtual=1;
	int repeticoes=10;//10
	int inicio = 1;
	int q=2;

	for (cont=1;cont<=repeticoes;cont++){
		
		termoAtual = termoAtual * q;

		if (cont==1){
			termoAtual = inicio;
		}

		printf("PG   termo %2d : %4d\n",cont,termoAtual);
		yield();
	}
}

void fibonacci(void){
	int cont;
	int termoAtual=0;
	int anterior =0;
	int maisAnterior =0;
	int repeticoes = 12;//12

	for (cont=1;cont<=repeticoes;cont++){
		
		termoAtual = anterior + maisAnterior;
		maisAnterior = anterior;
		anterior = termoAtual;

		if (cont==1){
			termoAtual = 1;
			anterior = 1;
			maisAnterior = 0;
		}

		printf("Fibo termo %2d : %4d\n",cont,termoAtual);
		yield();
	}
}

void defNumerosTriangulares(void){
	int cont;
	int termoAtual=1;
	int repeticoes =6;//6

	for (cont=1;cont<=repeticoes;cont++){
		
		termoAtual = (cont * (cont+1))/2;

		printf("Tri  termo %2d : %4d\n",cont,termoAtual);
		yield();
	}
}

void main(){
	
	TCB_t *arit_tcb, *geo_tcb, *fibo_tcb, *tri_tcb;

	if (CreateFila2(&aptos) != 0) printf("erro ao criar fila executando \n");

	getcontext(&main_ctx);

	getcontext(&forwarder_ctx);
	forwarder_ctx.uc_stack.ss_sp = malloc(SIGSTKSZ);
	forwarder_ctx.uc_stack.ss_size= SIGSTKSZ;
	forwarder_ctx.uc_link = 0;
	makecontext(&forwarder_ctx, forwarder,0);

	//cria contexto progressao aritmetica
	getcontext(&arit_ctx);
	arit_ctx.uc_stack.ss_sp = malloc(SIGSTKSZ);
	arit_ctx.uc_stack.ss_size= SIGSTKSZ;
	arit_ctx.uc_link = &forwarder_ctx;
	makecontext(&arit_ctx, progAritmetica,0);
	arit_tcb = (TCB_t *)malloc(sizeof(TCB_t));
	arit_tcb->tid = ids ++;
	arit_tcb->state=2;//executando
	arit_tcb->context = arit_ctx;
	EXECUTANDO = arit_tcb;

	//cria contexto progressao geometrica
	getcontext(&geo_ctx);
	geo_ctx.uc_stack.ss_sp = malloc(SIGSTKSZ);
	geo_ctx.uc_stack.ss_size= SIGSTKSZ;
	geo_ctx.uc_link = &forwarder_ctx;
	makecontext(&geo_ctx, progGeometrica,0);
	geo_tcb = (TCB_t *)malloc(sizeof(TCB_t));
	geo_tcb->tid = ids ++;
	geo_tcb->state=1;//apto
	geo_tcb->context = geo_ctx;
	AppendFila2(&aptos, (void*)geo_tcb);
	
	//cria contexto fibonacci
	getcontext(&fibo_ctx);
	fibo_ctx.uc_stack.ss_sp = malloc(SIGSTKSZ);
	fibo_ctx.uc_stack.ss_size= SIGSTKSZ;
	fibo_ctx.uc_link = &forwarder_ctx;
	makecontext(&fibo_ctx, fibonacci,0);
	fibo_tcb = (TCB_t *)malloc(sizeof(TCB_t));
	fibo_tcb->tid = ids ++;
	fibo_tcb->state=1;//apto
	fibo_tcb->context = fibo_ctx;
	AppendFila2(&aptos, (void*)fibo_tcb);
	
	//cria contexto numeros triangulares
	getcontext(&tri_ctx);
	tri_ctx.uc_stack.ss_sp = malloc(SIGSTKSZ);
	tri_ctx.uc_stack.ss_size= SIGSTKSZ;
	tri_ctx.uc_link = &forwarder_ctx;
	makecontext(&tri_ctx, defNumerosTriangulares,0);
	tri_tcb = (TCB_t *)malloc(sizeof(TCB_t));
	tri_tcb->tid = ids ++;
	tri_tcb->state=1;//apto
	tri_tcb->context = tri_ctx;
	AppendFila2(&aptos, (void*)tri_tcb);
	

	swapcontext(&main_ctx, &arit_ctx);

}