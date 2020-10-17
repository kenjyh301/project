#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "dataProcess.h"



void* mdataSend1(void*){
	while(1){
		printf("hello1");
		sleep(1);
	}
}

int main() {
//	time_t t= time(0);
//	printf("aaa:%ld",t);

//	while(1){
//		printf("writing to socket\n");
//		inet_write(sock,my_data,6);
//		sleep(5);
//	}



	pthread_create(&sendThread,NULL,mdataRead,NULL);
	while(1);
	return 0;
}







