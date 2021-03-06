/*
 * cdata.h: arquivo de inclus�o de uso apenas na gera��o da libpithread
 *
 * Esse arquivo pode ser modificado. ENTRETANTO, deve ser utilizada a TCB fornecida
 *
 * Vers�o de 25/04/2017
 *
 */
#include <ucontext.h>

#ifndef __cdata__
#define __cdata__

#define	PROCST_CRIACAO	0
#define	PROCST_APTO	1
#define	PROCST_EXEC	2
#define	PROCST_BLOQ	3
#define	PROCST_TERMINO	4

/* N�O ALTERAR ESSA struct */
typedef struct s_TCB { 
	int		tid; 		// identificador da thread
	int		state;		// estado em que a thread se encontra
					// 0: Cria��o; 1: Apto; 2: Execu��o; 3: Bloqueado e 4: T�rmino
	int 		ticket;		// "bilhete" de loteria da thread, para uso do escalonador
	ucontext_t 	context;	// contexto de execu��o da thread (SP, PC, GPRs e recursos) 
} TCB_t; 

//usamos esta estrutura pra colocar na lsita de bloqueados
typedef struct s_JOINBLOCK{
	TCB_t *tcb;	//thread bloqueada;
	int id_tcb_dominante; //id da thread que esta dominando o recurso do qual a tcb nesta estrutura precisa
} JOINBLOCK;

FILA2 aptos0;
FILA2 aptos1;
FILA2 aptos2;
FILA2 aptos3;
FILA2 bloqueados;

extern int ids;
extern ucontext_t *FORWARDER;
extern TCB_t *EXECUTANDO;

void init();
void forwarder();
TCB_t * escalonador();
void removeDeApto(int id, int prioridade);
void removeDeBloqueado(int id);
int verificaTid(int tid);
#endif
