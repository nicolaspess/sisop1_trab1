#include "../include/support.h"
#include "../include/cdata.h"
#include "../include/cthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>


//verifica se pelo menos uma das filas passada é vazia
	// 0: todas vazias
	// 1: pelo menos uma não vazia
int VerificaFilasVazia(PFILA2 pFila1, PFILA2 pFila2, PFILA2 pFila3){

	int a,b,c;
	int ret;
	a = FirstFila2(pFila1); //se 0 -> não vazia
	b = FirstFila2(pFila2);
	c = FirstFila2(pFila3);  
	if( a==0 || b==0 || c==0 ){
		//alguma não está vazia
		ret = 1;

	}else{
		//todas vazias
		ret = 0;
	}
	return ret;
}

int main() {
	unsigned int random_number;
	int quant_TCB = 0;
	int ids = 0;
	char acao[50];
	int terminados =0;

	//ponteiro das filas
	FILA2 aptos;
	FILA2 executando;
	FILA2 bloqueados;
	
	//criando as filas
	if (CreateFila2(&aptos) != 0) printf("erro ao criar fila aptos \n");
	if (CreateFila2(&executando) != 0) printf("erro ao criar fila executando \n");
	if (CreateFila2(&bloqueados) != 0) printf("erro ao criar fila bloqueados \n");
	//criadas filas vazias

	int ignora_criacao = 0;
	int i;
	//roda enquanto tiver menos de 30TCBS e depois: até quando apto || executando || bloqueado forem não vazias 
	for(i=0; ignora_criacao == 0 || VerificaFilasVazia(&aptos, &executando, &bloqueados) == 1; i++){

		random_number = Random2() % 12;

		if(random_number == 0 || random_number == 6){
			//criar um elemento TCB (definido em cdata.h) e inserir na fila de apto(simula a criação de uma thread)
			//máximo 30 threads

			if(ignora_criacao == 0){

				TCB_t *newTCB;
				newTCB = (TCB_t *)(malloc(sizeof(TCB_t)));
				newTCB->tid = ids;
				newTCB->state = 1; //estado apto

				AppendFila2(&aptos, (void *)newTCB); //insere no fim da fila
				quant_TCB++;
				ids++;
				if(quant_TCB == 30)
					ignora_criacao = 1;
			}
			strcpy(acao, "cria TCB e coloca em apto");
			
		}else if(random_number == 1 || random_number == 7){
			//retirar um elemento TCB da fila de apto e inserir na fila de executando, se a fila executando estiver vazia, caso contrário, nada deve acontecer
			int n;
			n = FirstFila2(&executando);
			if(n != 0){
				//fila vazia ou erro
				FirstFila2(&aptos); //iterador no primeiro da fila de aptos
				TCB_t *tcb;
				tcb = (TCB_t *)GetAtIteratorFila2(&aptos); //primeiro da lista de aptos
				
				AppendFila2(&executando, (void *)tcb);//colocamos 'tcb' na lista de executando

				//retira o primeiro da fila da lista de aptos
				if(DeleteAtIteratorFila2(&aptos)){
					//erro
					printf("erro ao deletar primeiro da fila de aptos. \n");
				}
			}
			strcpy(acao, "retira TCB de apto e poe em executando");
			
		}else if(random_number == 2 || random_number == 8){
			//retirar o elemento TCB da fila de executando e liberar o espaço alocado pelo elemento TCB (simula o término de uma thread). Se a fila executando estiver vazia nada deve acontecer.
			int n;
			n = FirstFila2(&executando);
			if(n == 0){
				//fila não vazia
				TCB_t *tcb;
				tcb = (TCB_t *)GetAtIteratorFila2(&executando);
				free(tcb);
				DeleteAtIteratorFila2(&executando);
				//free(nodo);
				//quant_TCB--;
				terminados++;
			}
			
			strcpy(acao,"TCB sai de executando e termina");
			
		}else if(random_number == 3 || random_number == 9){
			//retirar um elemento TCB da fila executando e inserir na fila de bloqueado(simula uma thread sendo bloqueada). Se a fila de executando estiver vazia nada deve ser feito.
			int n;
			n = FirstFila2(&executando);
			if(n == 0){
				//fila não vazia
				TCB_t *tcb;
				tcb = (TCB_t *)GetAtIteratorFila2(&executando); //primeiro da lista de executando
				AppendFila2(&bloqueados, (void *)tcb);
				DeleteAtIteratorFila2(&executando);
			}
			strcpy(acao,"TCB sai de executanto e vai para bloqueado");
			
		}else if(random_number == 4 || random_number == 10){
			//Retirar o elemento TCB da fila de executando e inserir na fila de apto(simula a cedência voluntária do processador). Se a fila de executando estiver vazia nada deve ser feito.
			int n;
			n = FirstFila2(&executando);
			if(n == 0){
				//fila não vazia
				TCB_t *tcb;
				tcb = (TCB_t *)GetAtIteratorFila2(&executando); //primeiro da lista de executando
				AppendFila2(&aptos, (void *)tcb);
				DeleteAtIteratorFila2(&executando);
			}
			strcpy(acao, "TCB sai de executando e vai para apto");

		}else if(random_number == 5 || random_number == 11){
			//Retirar um elemento TCB da fila bloqueado e inserir na fila de apto. Se a fila bloqueado estiver vazia, nada deve ser feito.
			int n;
			n = FirstFila2(&bloqueados);
			if(n == 0){
				//fila não vazia
				TCB_t *tcb;
				tcb = (TCB_t *)GetAtIteratorFila2(&bloqueados); //primeiro da lista de bloqueados
				AppendFila2(&aptos, (void *)tcb);
				DeleteAtIteratorFila2(&bloqueados);
			}
			strcpy(acao, "TCB sai de bloqueado e vai para apto");
		}

		//teste para ver quantos estão em apto
		//printf("FILA APTOS ->identificadores: ");
		FirstFila2(&aptos);
		TCB_t *tcb;
		tcb = (TCB_t *)GetAtIteratorFila2(&aptos);
		int quant_aptos=0;
		while(tcb != NULL){
			//printf(" %d;", tcb->tid);
			NextFila2(&aptos);
			tcb = (TCB_t *)GetAtIteratorFila2(&aptos);
			quant_aptos++;
		}	

		//teste para ver fila executando
		//printf("FILA EXECUTANDO ->identificadores: ");
		FirstFila2(&executando);
		TCB_t *tcbExec;
		tcbExec = (TCB_t *)GetAtIteratorFila2(&executando);
		int quant_execs = 0;
		while(tcbExec != NULL){
			//printf(" %d;", tcbExec->tid);
			NextFila2(&executando);
			tcbExec = (TCB_t *)GetAtIteratorFila2(&executando);
			quant_execs++;
		}
	
		//teste para ver fila bloqueado
		//printf("FILA BLOQUEADO ->identificadores: ");
		FirstFila2(&bloqueados);
		TCB_t *tcbBloq;
		tcbBloq = (TCB_t *)GetAtIteratorFila2(&bloqueados);
		int quant_bloqs = 0;
		while(tcbBloq != NULL){
			//printf(" %d;", tcbBloq->tid);
			NextFila2(&bloqueados);
			tcbBloq = (TCB_t *)GetAtIteratorFila2(&bloqueados);
			quant_bloqs++;
		}

		printf("iteracao %d:    resto: %d    acao: %s \n", i, random_number, acao);
		printf("	Elementos em apto: %d\n", quant_aptos);
		printf("	Elementos em executando: %d\n", quant_execs);
		printf("	Elementos em bloqueado: %d\n", quant_bloqs);
		printf("	Elementos em terminados: %d\n", terminados);
	}
}