/*
 * format.c
 *
 *  Created on: 28 May 2019
 *      Author: lxmi
 */


/*
 * filter.c
 *
 *  Created on: 14 Mar 2019
 *      Author: lxmi
 */

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */

#include "format.h"
const int N = 256;
void format1(char *buf) {

	for (int i = 2; i < N; ++i) { //Actual command starts after "@ "- 2 places skipped on array
		//printf("%c", buf[i]);
		if (buf[i] == '/') {		//Replace Line feed or space with a carry
			buf[i] = 13;			//Carry
			buf[i - 2] = buf[i];
			buf[i] = '\0';		 //Resetting last 2 places to 0
			buf[i - 1] = '\0'; 	//Resetting last 2 places to 0
			break;
		}
		buf[i - 2] = buf[i]; //Shifting 2 places to left after removing "@ "

	}
}
void format2(char *buf) {

	for (int i = 1; i < N; ++i) { //Actual command starts after "@ "- 2 places skipped on array
		//printf("%c", buf[i]);

		buf[i - 1] = buf[i];
		if (buf[i] == 32) {			//Replace Spaces with comma
			buf[i] = 44;			//Comma
			buf[i - 1] = buf[i];

		} else if (buf[i] == 13) {
			buf[i] = '\0';//Replace carry with new line
			buf[i - 1] = buf[i];
			break;
		}
	}

}
