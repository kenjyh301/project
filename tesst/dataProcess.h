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
#include <time.h>

#define NUM_BYTES 170
#define DATA_BASE 0x080586b4
#define PORT 5353
#define SA struct sockaddr
#define azd 10
#define SAMPLE_TIME 16000
#define SLEEP_TIME SAMPLE_TIME/4

pthread_mutex_t data_mutex;
pthread_t sendThread;
pthread_t readThread;
int init_send = 0;
int ppi51_pid = 0;
int ppi51_chid = 0;
char* tmp;
char data[NUM_BYTES+2];
int connfd;
clock_t clk=0,clk_pr=0;

void get_ppi51_pid() {
	char tmp[512];
	FILE* fp = popen("pidin -n uk51. -p ppi51 -F '%a %J %B' | grep RECEIVE",
			"r");
	fscanf(fp, "%d %s %d", &ppi51_pid, tmp, &ppi51_chid);
	printf("ppi51 pid %d, chid %d\n", ppi51_pid, ppi51_chid);
	pclose(fp);
}

void get_ppi51_source(char* s) {
	get_ppi51_pid();
	if (ppi51_pid <= 0) {
		printf("get ppi51 pid error\n");
		return;
	}
	sprintf(s, "//net//uk51.//proc//%d//as", ppi51_pid);
	printf("data source file:%s\n");
}

void print_current_data() {
	char m[50];
	get_ppi51_source(m);
	FILE* f = fopen((char*) m, "r");
	if (f == NULL)
		printf("error\n");
	else
		printf("success\n");

	long addr;

	char data[4];
	char data1;
	//	fread(data,4,1,f);
	//	printf("%x\n",data);
	//	fread(data,4,1,f);
	//	printf("%x\n",data);
	while (1) {
		addr = (long) 0x080586b4;
		fseek(f, (long int) addr, SEEK_SET);
		for (int i = 0; i < 20; i++) {
			//		memset(data,0,4);
			fread(data, 4, 1, f);
			printf("0x");
			for (int j = 4; j > 0; j--) {
				if ((unsigned char) data[j - 1] < 16)
					printf("0");
				printf("%hhx", data[j - 1]);
			}
			printf("\n");
			//		fread(&data1,1,1,f);
			//		printf("%x\n",data1&0xff);
		}
		printf("%x\n", ftell(f));
		usleep(1000);
	}
}

void inet_init(int &connfd) {
	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);
	if (bind(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) != 0) {
		printf("socket bind failed\n");
	} else {
		printf("socket bind success\n");
	}
	listen(sock, 10);
	printf("listening...\n");
	connfd = accept(sock, NULL, NULL);
	printf("client accepted\n");
	if (connfd < 0) {
		printf("server acccept failed\n");
		exit(0);
	} else
		printf("server acccept the client\n");
	// Function for chatting between client and server
}

void inet_write(int connfd, char* data, int length) {
	write(connfd, data, length);
}

void* mdataSend(void*) {
	FILE* f1 = fopen("//minh_data//write_data", "w");
	if (f1 == NULL)
		printf("open file error\n");
	else
		printf("open file success\n");

	while (1) {
		pthread_mutex_lock(&data_mutex);
		//do send here
		fseek(f1, 0, SEEK_CUR);
		for (int i = 0; i < NUM_BYTES+2; i++) {
			fwrite(&data[i], 1, 1, f1);
		}
//		printf("%d\n", ftell(f1));

		inet_write(connfd, data, NUM_BYTES+2);
		/////////////////////////////////////////
		pthread_mutex_unlock(&data_mutex);
		usleep(SLEEP_TIME);
	}

}

void* mdataRead(void*) {
//	char m[50];
//	get_ppi51_source(m);
//	FILE* f = fopen(m, "r");
	FILE* f = fopen("//net//uk51.//proc//581669//as", "r");
	if (f == NULL)
		printf("open ppi51 error\n");
	else
		printf("open ppi51 success\n");

	long addr = DATA_BASE;

	pthread_mutex_init(&data_mutex, NULL);

	inet_init(connfd);
	data[0]=0xAA;
	data[1]=0x55;

	while (1) {
		pthread_mutex_lock(&data_mutex);
		//do read here
//		usleep(SAMPLE_TIME-(clock()-clk));
		printf("%ld\n",clock()-clk);
		clk=clock();
		fseek(f, (long int) addr, SEEK_SET);
		memset(&data[2], 0, NUM_BYTES);
		for (int i = 0; i < NUM_BYTES; i++) {
			fread(&data[i+2], 1, 1, f);
		}
		printf("%x\n", ftell(f));
		//		sleep(20);

		/////////////////////////////////////
		if (!init_send) {
			init_send = 1;
			pthread_create(&sendThread, NULL, mdataSend, NULL);
		}
		pthread_mutex_unlock(&data_mutex);
		//		sleep(SLEEP_TIME);
		usleep(SLEEP_TIME);
	}

}

#endif /* DATAPROCESS_H_ */
