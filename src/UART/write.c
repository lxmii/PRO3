/*
 * write.c
 *
 *  Created on: 10 May 2019
 *      Author: lxmi
 */

#ifndef SRC_WRITE_C_
#define SRC_WRITE_C_
#include "readwrite.h"
int uart;
volatile int W = 1;
void* Write(void *vargp) {
	while (1) { //syslog(LOG_NOTICE, "Ready to send next command in  file commands\n");
		while(ENABLE == 1) {
			char buf[N];
			memset(buf, 0, sizeof(char) * N);
			if (TEST != 1) {
				fflush(commands);
				fgets(buf, N, commands); //Terminates with newline or Line feed
				if (buf[0] != '@') { //All commands to the machine in the file start with symbol @
					continue;
				}
				format1(buf); // replaces buf with - buf minus first 2 characters works with pointers so no need to return values
				syslog(LOG_NOTICE, "Ready to write %s to machine\n", buf);
			} else { //Ongoing test, dont read further commands
				syslog(LOG_NOTICE, "Ongoing Test\n");

				W = 0;

				continue;
			}
			while (1) {
				if (pthread_mutex_lock(&Lock) == 0) { //When successful, other functions/threads requiring pthread_mutex_lock of Lock will pause
					//printf("Mutex locked by write thread\n");
					if (W == 1) {
					//	syslog(LOG_NOTICE, "W is 1 man\n");
						if (TEST != 1) {
							//	syslog(LOG_NOTICE, "Test not 1// so sending \n");
							if (buf[0] == 'S') //Test start command starts with S, if thats the case, do not read next command
									{
								TEST = 1; //Setting flag to indicate read test that it the next is a test
							} //syslog(LOG_NOTICE, "Sending command %s to machine\n",buf);
							dprintf(uart, "%s", buf);
						}
						W = 0; //syslog(LOG_NOTICE, "Setting W to %d leaving write thread\n",W);
						//syslog(LOG_NOTICE,
							//	"Relinquishing mutex from write thread\n");
						pthread_mutex_unlock(&Lock);
						break;

					} else { //W flag not yet reset by read file
						pthread_mutex_unlock(&Lock);
						usleep(WAIT);
					}
				} else {
					//syslog(LOG_NOTICE,"Mutex lock on write unsuccessful");
					pthread_mutex_unlock(&Lock);
					usleep(WAIT);
				}
			}

		}
	}
}

#endif /* SRC_WRITE_C_ */
