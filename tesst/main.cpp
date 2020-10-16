#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_BYTES 20
#define DATA_BASE 0x080586b4;

int main() {
	FILE* f = fopen("//net//uk51.//proc//581669//as", "r");

	if (f == NULL)
		printf("error\n");
	else
		printf("success\n");
	long* addr;
	char data[NUM_BYTES];
	while (1) {
		addr = (long*) DATA_BASE;
		fseek(f, (long int) addr, SEEK_SET);
		for (int i = 0; i < NUM_BYTES; i++) {
			//		memset(data,0,4);
			fread(&data[i], 1, 1, f);
			printf("0x");
			printf("%hhx",data[i]);
			printf("\n");
		}
		printf("%x\n", ftell(f));
		sleep(2);
	}

	return 0;
}
