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
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define NUM_BYTES 5
#define DATA_BASE 0x080586b4;
#define PORT 8080
#define SA struct sockaddr
#define SLEEP_TIME 2

pthread_mutex_t data_mutex;
pthread_t sendThread;
pthread_t readThread;
int init_send=0;

char data[NUM_BYTES];

void inet_init(int &connfd) {
	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serv_addr;
	bzero(&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr= htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);
	if(bind(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))!=0){
		printf("socket bind failed\n");
	}else{
		printf("socket bind success\n");
	}
	listen(sock, 10);
	connfd = accept(sock, NULL, NULL);
	if (connfd < 0) {
		printf("server acccept failed\n");
		exit(0);
	} else
		printf("server acccept the client\n");
	// Function for chatting between client and server
}

void inet_write(int connfd,char* data,int length){
	write(connfd,data,length);
}


void* mdataSend(void*) {
	FILE* f1= fopen("//minh_data//write_data","w");
	int connfd;
	inet_init(connfd);

	while (1) {
		pthread_mutex_lock(&data_mutex);
		//do send here
		fseek(f1,0,SEEK_SET);
		if (f1 == NULL)
			printf("open write_data error\n");
		else{
			printf("open write_data success\n");
		}

		for (int i = 0; i < NUM_BYTES; i++) {
			fwrite(&data[i], 1, 1, f1);
		}

		inet_write(connfd,data,NUM_BYTES);


		/////////////////////////////////////////
		pthread_mutex_unlock(&data_mutex);
		sleep(SLEEP_TIME);
	}

}

void* mdataRead(void*) {
	FILE* f = fopen("//net//uk51.//proc//585765//as", "r");
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
		sleep(SLEEP_TIME);
	}

}

#endif /* DATAPROCESS_H_ */
