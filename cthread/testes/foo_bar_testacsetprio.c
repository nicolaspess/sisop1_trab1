#include <stdio.h>
#include "../include/support.h"
#include "../include/cthread.h"
#define MAX 100
int i=1;
int id0, id1;

void* foo(void *arg){
	int i=1;
	
	for(;i<MAX;i++){
		if(i==30){
			csetprio(id0,2);
			
		}
		
		if(i%15==0){
			printf("FOO (i=%d)\n",i);
			cyield();
		}
	}
	return NULL;
}

void* bar(void *arg){
	int i=1;
	
	for(i++;i<MAX;i++){
		if(i==75)
			csetprio(id1,3);
		if(i%15==0){
			printf("===========BAR (i=%d)\n",i);
			cyield();
		}
	}
	return NULL;
}

int main(){

	printf("\n\n");
	
	id0 = ccreate(foo, (void *)&i,0);
	id1 = ccreate(bar, (void *)&i,0);	
	
	csetprio(id1,1);
	cjoin(id0); cjoin(id1);

	

	printf("\n\n");
	return 0;
}