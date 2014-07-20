/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "io/stdfile.h"
//#include "dev/uart/uart.h"


/* write to ftdi uart */
static int _put_ftdi(char c, FILE *f) {
	/* unused */
	(void)f;
	
	/* convert LF to CRLF */
	/*if (c == '\n' && !uart_write('\r')) {
		return _FDEV_ERR;
	}
	
	if (uart_write(c)) {
		return 0;
	} else {*/
		return _FDEV_ERR;
	/*}*/
}

/* read from ftdi uart */
static int _get_ftdi(FILE *f) {
	/* unused */
	(void)f;
	
	char c;
	/*if (uart_read(&c)) {
		return (int)c;
	} else {*/
		return _FDEV_ERR;
	/*}*/
}


/* open std{in,out,err} and other files */
void stdfile_open(void) {
	(void)fdevopen(_put_ftdi, _get_ftdi);
}

/* close all files */
void stdfile_close(void) {
	fclose(stdin);
}
