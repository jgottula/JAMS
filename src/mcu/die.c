/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "mcu/die.h"
//#include "dev/uart/uart.h"


noreturn void die(void) {
	//uart_flush();
	
	cli();
	
	for ( ;; );
}
