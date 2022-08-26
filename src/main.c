/*
 * main.c
 *
 *  Created on: 26 Dec 2018
 *      Author: lxmi
 */





/*
 * main.c
 *
 *  Created on: 26 Dec 2018
 *      Author: lxmi
 */

#include "uart.h"

#define WAIT 1000

volatile int R = 1;
void *Read(void *vargp);
void *Write(void *vargp);

extern pthread_mutex_t Lock;

int main(void) {

	uart = open_port();
	initSerial();
	printf("Hello World\n");
	uart = open_port();
	void *funcs[] = { Write, Read };
	pthread_t tid[2];
	pthread_attr_t attr;
	void *status;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	int err;
	pthread_mutex_init(&Lock, NULL);
	for (i = 0; i < 2; i++) {
		err = pthread_create(&tid[i], &attr, funcs[i], NULL);
		if (err) {
			perror("Couldn't create thread\n");
		}
		printf("Created thread with threadID: %ld\n", tid[i]);

	}
	pthread_attr_destroy(&attr);
	pthread_join(tid[0], &status);
	pthread_cancel(tid[1]);
	//close(uart);
	printf("Bye\n");

}
void* Write(void *vargp) {

	while (1) {
		char buf[2];
		buf[0]=getchar();
		if (buf[0] != 'q') //Check later
				{
			while (1) {

				if (pthread_mutex_lock(&Lock) == 0) {
					if (R == 1) {
						res = write(uart, buf, 1);
						buf[res] = 0; /* set end of string, so we can scanf */
						R = 0;
						pthread_mutex_unlock(&Lock);
						break;

					} else {
						pthread_mutex_unlock(&Lock);
						usleep(WAIT);
					}

				} else
					printf("Not Ready to write/Tx, lock unsuccessful\n");
			}
		} else {
			pthread_exit((void*) 0);
		}

	}

}

void* Read(void *vargp) {

	char buf[2];
	int res;
	while (1) {

			while (1) {

				if (pthread_mutex_lock(&Lock) == 0) {

					if (R == 0) {
						memset(buf, 0, sizeof(char)*2);
						//printf("Entering read\n");
						res = read(uart, buf, 1);
						//printf("%d", res);
						buf[res] = 0; /* set end of string, so we can printf */
						printf("%s", buf);
						R = 1;
						pthread_mutex_unlock(&Lock);
						break;

					}
					else {
					//	printf("Not Ready to read/Rx, flag reset\n");
						pthread_mutex_unlock(&Lock);
						usleep(WAIT); // Wait a tick and try again
					}

				} else
					printf("Not Ready to read/Rx\n");

			}


	}

}




