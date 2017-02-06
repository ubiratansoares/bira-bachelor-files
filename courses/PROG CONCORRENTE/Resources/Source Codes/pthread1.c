#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void* task(void){
	
	printf("\n>> TID = %d", pthread_self());
	pthread_exit(0);
	
}

int main(void){
	
	
	pthread_t tid;
	pthread_attr_t attr;
	
	
	//pthread_attr_init(&attr);
	//pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);

	printf("\n>> PID = %d", pthread_self());

	pthread_create(&tid,NULL,task,NULL);

	pthread_join(tid,NULL);
	
	getchar();getchar();
	
	return 0;
	
}