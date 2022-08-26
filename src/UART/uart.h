/*
 * uart.h
 *
 *  Created on: 26 Dec 2018
 *      Author: lxmi
 */

#ifndef UART_H_
#define UART_H_

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <pthread.h>
#include <ctype.h>
#include <time.h>
#include "read.h"
#include "write.h"
#include "daemonise.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <signal.h>
int uart, res, i;

void initSerial(void);
/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */

int
open_port(void);
void close_port(void);

#endif /* UART_H_ */
