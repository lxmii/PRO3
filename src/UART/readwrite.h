/*
 * readwrite.h
 *
 *  Created on: 10 May 2019
 *      Author: lxmi
 */

#ifndef SRC_READWRITE_H_
#define SRC_READWRITE_H_
#include "uart.h"
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <pthread.h>
#include <ctype.h>
#include "format.h"

#define WAIT 1000	//1ms
#define WAIT4TEST 50000 //50 ms
extern	pthread_t tid[2];//declared in main.c
extern volatile int W;//W=1 in the start of test ensures that write takes place before read
extern volatile int TEST;//TEST=1 indicates that a test should either begin or that a test is ongoing
extern volatile int ENABLE;
//volatile long int posCommands, postestResults;
#define WAIT 1000
extern pthread_mutex_t Lock;//declared on main
FILE *commands;
FILE *replies;
FILE *testResults;
extern int uart, res;//declared in uart.h


#endif /* SRC_READWRITE_H_ */
