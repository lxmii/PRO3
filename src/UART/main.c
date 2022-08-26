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

/*
 * sudo tail -f /var/log/messages|grep DY34
 * kill -SIGHUP PID
 * kill -SIGUSR1 PID
 * kill -SIGUSR2 PID
 *
 *
 */
#include "readwrite.h"
#include "uart.h"

#define SESSIONUP SIGUSR1
#define SESSIONDWN SIGUSR2
void *funcs[] = { Read, Write };
pthread_mutex_t Lock;
pthread_t tid[2];
int i;
volatile int ENABLE = 0;
static void Terminate(int sig, siginfo_t *siginfo, void *context) {
	// Send a cancellation signal to the listener

	syslog(LOG_NOTICE, "Terminating daemon\n");
	pthread_cancel(tid[0]);
	pthread_cancel(tid[1]);
	fclose(replies);
	fclose(commands);
	fclose(testResults);
	close_port();
	closelog();
	exit(0);
}

static void sessionUp(int sig, siginfo_t *siginfo, void *context) {
	{ // Send a cancellation signal to the listener

		uart = open_port();
		initSerial();
		if((replies = fopen("/var/www/tt/replies", "w")))
		{
			syslog(LOG_NOTICE, "Opening replies");
		}//appends to the file, creates file if not already created
		if((commands = fopen("/var/www/tt/commands", "r")))
		{
			syslog(LOG_NOTICE, "Opening commands");
		}
		if((testResults = fopen("/var/www/tt/testResults", "w")))
		{
			syslog(LOG_NOTICE, "Opening testresults");
		}
		W=1;
		ENABLE = 1;
	}
}
static void sessionDown(int sig, siginfo_t *siginfo, void *context) {
	{ // Send a cancellation signal to the listener
		syslog(LOG_NOTICE, "Closing replies, commands and testresults");
		fclose(replies);
		fclose(commands);
		fclose(testResults);
		W = 1;
		TEST=0;
		ENABLE = 0;
	}
}
int main(int argc, char *argv[]) {

	struct sigaction act;

	daemonise();
	setlogmask(LOG_UPTO(LOG_NOTICE));
	openlog("CTMcom", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	syslog(LOG_NOTICE, "Program started by User %d", getuid());
	syslog(LOG_NOTICE, "Standard file descriptors closed.");

	memset(&act, '\0', sizeof(act));
	act.sa_sigaction = &Terminate;
	// The SA_SIGINFO flag tells sigaction() to use the sa_sigaction
	// field, not sa_handler.
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SIGHUP, &act, NULL) < 0) {
		syslog(LOG_NOTICE, "sigaction failed to install");
	}
	syslog(LOG_NOTICE,
			"sigaction installed successfully. Kill me using the SIGHUP signal.");

	memset(&act, '\0', sizeof(act));
	act.sa_sigaction = &sessionUp;
	// The SA_SIGINFO flag tells sigaction() to use the sa_sigaction
	// field, not sa_handler.
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SESSIONUP, &act, NULL) < 0) {
		syslog(LOG_NOTICE, "sigaction failed to install");
	}
	syslog(LOG_NOTICE,
			"sigaction installed successfully. Start session with signal kill- USR1 <pid>.");

	memset(&act, '\0', sizeof(act));
	act.sa_sigaction = &sessionDown;
	// The SA_SIGINFO flag tells sigaction() to use the sa_sigaction
	// field, not sa_handler.
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SESSIONDWN, &act, NULL) < 0) {
		syslog(LOG_NOTICE, "sigaction failed to install");
	}
	syslog(LOG_NOTICE,
			"sigaction installed successfully. Start session with signal- USR2.");

	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	int err;
	pthread_mutex_init(&Lock, NULL);
	for (i = 0; i < 2; i++) {
		err = pthread_create(&tid[i], &attr, funcs[i], NULL);
		if (err) {
			perror("Couldn't create thread\n");
		}
		syslog(LOG_NOTICE, "Created thread with threadID: %ld\n", tid[i]);

	}
	pthread_attr_destroy(&attr);

//	pthread_join(tid[0], &status);
//	pthread_cancel(tid[1]);
	while (1) {
		usleep(1000);
	}

}

