/*
 * uart.c
 *
 *  Created on: 26 Dec 2018
 *      Author: lxmi
 */

#include "uart.h"

void initSerial(void) {

#define BAUDRATE B9600
	struct termios newtio;
	newtio.c_cflag = BAUDRATE | CRTSCTS | CS7 | CLOCAL | CREAD | PARENB; //7 bit even parity 9.6k baud
	newtio.c_cflag &= ~(CSTOPB); //1 stop bit
	//newtio.c_oflag = CR0|ONLRET|OFDEL;//ONLRET a new line appends carriage return to be sent out does not work :(
	//OFDEL sets null to unused bits
	newtio.c_oflag = CR0 | OFDEL;
	newtio.c_iflag = (ICRNL | INPCK | PARMRK);//Carriage return from machine gets converted to New line on computer
//INPCK in enable i/p parityy checking
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 1;
	tcflush(uart, TCIFLUSH);
	tcsetattr(uart, TCSANOW, &newtio);
	syslog(LOG_NOTICE, "UART set up\n");

}
int open_port(void) {
	int fd; /* File descriptor for the port */

	fd = open("/dev/ttyS4", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		perror("open_port: Unable to open /dev/ttyf	 - ");
	} else
		fcntl(fd, F_SETFL, 0);

	return (fd);
}

void close_port(void) {
close("/dev/ttyS4");
}
