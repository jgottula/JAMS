/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "std.h"
//#include "dev/uart/uart.h"
#include "mcu/clock.h"
#include "mcu/pmic.h"


int main(void) {
	mcu_setup_xtal();
	mcu_setup_pll();
	mcu_use_pll();
	
	pmic_init();
	
	//uart_init();
	stdfile_open();
	
	
	rtos_init();
	
	
	idle();
}
