#include "../include/support.h"
#include "../include/cdata.h"
#include "../include/cthread.h"
#include <stdio.h>
#include <string.h>
#include <ucontext.h>

int main() {
	unsigned int random_number;
	int quant_TCB = 0;
	int ids = 0;
	char acao[50];

	//ponteiro das filas
	FILA2 aptos;
	FILA2 executando;
	FILA2 bloqueados;
	
	//criando as filas
	if (CreateFila2(&aptos) != 0) printf("erro ao criar fila aptos \n");
	if (CreateFila2(&executando) != 0) printf("erro ao criar fila executando \n");
	if (CreateFila2(&bloqueados) != 0) printf("erro ao criar fila bloqueados \n");

	//criadas filas vazias

	//roda enquanto apto || executando || bloqueado forem não vazias
	int i;
	for(i=0; i<30 ;i++){

		random_number = Random2() % 12;

		if(random_number == 0 || random_number == 6){
			//criar um elemento TCB (definido em cdata.h) e inserir na fila de apto(simula a criação de uma thread)
			//máximo 30 threads
			if(quant_TCB < 30){

				TCB_t *newTCB;
				newTCB = (TCB_t *)(malloc(sizeof(TCB_t)));
				newTCB->tid = ids;
				newTCB->state = 1; //estado apto

				//criar novo NODO
				NODE2 *newNode; 
				newNode = (NODE2 *)(malloc(sizeof(NODE2)));
				newNode->node = (void *)newTCB;

				//verificar se fila vazia. se nao vazia pega o ultimo nodo pra botar no 'ant' do nodo
				int n = LastFila(&aptos);
				if(n == 0){
					//lista não vazia
					//pega o ultimo nodo
					NODE2 *last;
					last = (NODE2 *)GetAtIteratorFila2(&aptos);
					newNode->ant = &last;
					newNode->next = NULL;
				}else{
					//lista vazia. só add
					newNode->ant = NULL;
					newNode->next = NULL;
				}

				//insere no fim da fila
				AppendFila2(&aptos, (void *)newNode);
				quant_TCB++;
				ids++;
			}
			strcpy(acao, "cria TCB e coloca em apto");
			
		}else if(random_number == 1 || random_number == 7){
			//retirar um elemento TCB da fila de apto e inserir na fila de executando, se a fila executando estiver vazia, caso contrário, nada deve acontecer
			strcpy(acao, "retira TCB de apto e poe em executando");
			
		}else if(random_number == 2 || random_number == 8){
			//retirar o elemento TCB da fila de executando e liberar o espaço alocado pelo elemento TCB (simula o término de uma thread). Se a fila executando estiver vazia nada deve acontecer.
			strcpy(acao,"TCB sai de executando e termina");
			
		}else if(random_number == 3 || random_number == 9){
			//retirar um elemento TCB da fila executando e inserir na fila de bloqueado(simula uma thread sendo bloqueada). Se a fila de executando estiver vazia nada deve ser feito.
			
			strcpy(acao,"TCB sai de executanto e vai para bloqueado");
			
		}else if(random_number == 4 || random_number == 10){
			//Retirar o elemento TCB da fila de executando e inserir na fila de apto(simula a cedência voluntária do processador). Se a fila de executando estiver vazia nada deve ser feito.
			strcpy(acao, "TCB sai de executando e vai para apto");

		}else if(random_number == 5 || random_number == 11){
			//Retirar um elemento TCB da fila bloqueado e inserir na fila de apto. Se a fila bloqueado estiver vazia, nada deve ser feito.
			strcpy(acao, "TCB sai de bloqueado e vai para apto");
			
		}
		//printf("iteracao %d:    resto: %d    ação: %s \n", i,random_number,acao);
		//printf("	Elementos em apto: %d\n", );
		//printf("	Elementos em executando: %d\n", );
		//printf("	Elementos em bloqueado: %d\n", );
		//printf("	Elementos em terminados: %d\n", );

	}

}