/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "mcu/reset.h"
//#include "dev/uart/uart.h"


noreturn void reset(void) {
	//uart_flush();
	
	cli();
	
	/* invoke a software reset */
	CCP = CCP_IOREG_gc;
	RST.CTRL |= RST_SWRST_bm;
	
	for ( ;; );
}
