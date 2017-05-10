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


void progAritmetica(int repeticoes, int inicio, int r){
	int cont;
	int termoAtual=0;

	for (cont=1;cont<=repeticoes;cont++){

		termoAtual = termoAtual + r;

		if (cont==1){
			termoAtual = inicio;
		}

		printf("PA   termo %2d : %4d \n",cont,termoAtual);
	}

}

void progGeometrica(int repeticoes, int inicio, int q){
	int cont;
	int termoAtual=1;

	for (cont=1;cont<=repeticoes;cont++){
		
		termoAtual = termoAtual * q;

		if (cont==1){
			termoAtual = inicio;
		}

		printf("PG   termo %2d : %4d\n",cont,termoAtual);
	}
}

void fibonacci(int repeticoes){
	int cont;
	int termoAtual=0;
	int anterior =0;
	int maisAnterior =0;

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
	}
}

void defNumerosTriangulares(int repeticoes){
	int cont;
	int termoAtual=1;

	for (cont=1;cont<=repeticoes;cont++){
		
		termoAtual = (cont * (cont+1))/2;

		printf("Tri  termo %2d : %4d\n",cont,termoAtual);
	}
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

	

	/*testando as funcoes
	printf("\n\n");
	progAritmetica(8,1, 4);				//8 termos
	printf("\n\n");
	progGeometrica(10, 1, 2);			//10 termos
	printf("\n\n");
	fibonacci(12);						//12 termos
	printf("\n\n");
	defNumerosTriangulares(6);			//6 numeros triangulares
	printf("\n\n");
	*/
}