/*
 * dataProcess.h
 *
 *  Created on: Oct 16, 2020
 *      Author: Minh
 */

#ifndef DATAPROCESS_H_
#define DATAPROCESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
//#include <socket.h>

#define NUM_BYTES 150
#define DATA_BASE 0x080586b4;

pthread_mutex_t data_mutex;
pthread_t sendThread;
pthread_t readThread;
int init_send=0;

char data[NUM_BYTES];

void* mdataSend(void*) {
	FILE* f1= fopen("//minh_data//write_data","w");
	while (1) {
		pthread_mutex_lock(&data_mutex);
		//do send here
		fseek(f1,0,SEEK_SET);
		if (f1 == NULL)
			printf("open write_data error\n");
		else{
			printf("open write_data success\n");
//			pthread_mutex_unlock(&data_mutex);
//			continue();
		}

		for (int i = 0; i < NUM_BYTES; i++) {
			fwrite(&data[i], 1, 1, f1);
		}


//		printf("hello\n");
		/////////////////////////////////////////
		pthread_mutex_unlock(&data_mutex);
		usleep(200);
	}

}

void* mdataRead(void*) {
	FILE* f = fopen("//net//uk51.//proc//581669//as", "r");
	if (f == NULL)
		printf("open ppi51 error\n");
	else
		printf("open ppi51 success\n");

	long addr = DATA_BASE;

	pthread_mutex_init(&data_mutex,NULL);
	while (1) {
		pthread_mutex_lock(&data_mutex);
		//do read here

		fseek(f, (long int) addr, SEEK_SET);
		memset(data,0,NUM_BYTES);
		for (int i = 0; i < NUM_BYTES; i++) {
			fread(&data[i], 1, 1, f);
		}
		printf("%x\n", ftell(f));
//		sleep(20);

		/////////////////////////////////////
		if(!init_send){
			init_send=1;
			pthread_create(&sendThread, NULL, mdataSend, NULL);
		}
		pthread_mutex_unlock(&data_mutex);
		usleep(200);
	}

}

#endif /* DATAPROCESS_H_ */
