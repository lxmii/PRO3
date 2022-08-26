/*
 * read.c
 *
 *  Created on: 10 May 2019
 *      Author: lxmi
 */
#include "read.h"
int res;

volatile int TEST = 0;

void* Read(void *vargp) {
	while (1) {
		while (ENABLE == 1) {

		//syslog(LOG_NOTICE, "Ready to read next message from machine\n");
			char buf[50];
			int res;

			while (1) {
		//		syslog(LOG_NOTICE, "Entered 2nd while loop\n");
				if (pthread_mutex_lock(&Lock) == 0) {
					//syslog(LOG_NOTICE, "Mutex locked\n");
					if (W == 0) {
					//	syslog(LOG_NOTICE, "W=0\n");

						memset(buf, 0, sizeof(char) * N);
						res = read(uart, buf, sizeof(char) * N);
						syslog(LOG_NOTICE, "Entering read\n");
						if (TEST != 1) {
							fflush(replies);
							fputs(buf, replies);
							fflush(replies);
							buf[res] = 0; /* set end of string, so we printf flushes out */
							if (buf[0] == EOF) {
								continue;
							}
						} else { //TEST==1
							syslog(LOG_NOTICE, "Ongoing test\n");
							if (buf[0] == 'S') { //Successful completion of test
								//TEST  = 0; //Reset to allow next write operation to take place
								fclose(replies);
								fclose(commands);
								fclose(testResults);
								W = 1;
								TEST = 0;
								ENABLE = 0;
								syslog(LOG_NOTICE,"Download testresult before starting new session ");
							}
							//buf[res] = 10;							//New line
							format2(buf);
							fputs((buf), testResults);
							fflush(testResults);

						}

						syslog(LOG_NOTICE, " reading %s from machine ", buf);
						W = 1;
					//	syslog(LOG_NOTICE,
					//			"Relinquishing mutex from read thread\n");
						pthread_mutex_unlock(&Lock);
						break;

					} else {
					//	syslog(LOG_NOTICE,
						//		"Not Ready to read/Rx, W flag set in write thread\n");
					//	syslog(LOG_NOTICE,
					//			"Relinquishing mutex from read thread\n");
						pthread_mutex_unlock(&Lock);
						usleep(WAIT); // Wait a tick and try again
					}

				} else {
				//	syslog(LOG_NOTICE, "Mutex lock on write unsuccessful");
					pthread_mutex_unlock(&Lock);
					usleep(WAIT);
				}

			}
		}

	}
}
