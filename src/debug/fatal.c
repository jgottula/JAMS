/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "debug/fatal.h"
#include "dev/uart/uart.h"


static noreturn void _fatal_common(const char *str, bool flash) {
	if (flash) {
		printf_P(PSTR("\nFATAL:\n%S\n"), str);
	} else {
		printf_P(PSTR("\nFATAL:\n%s\n"), str);
	}
	
	uart_flush();
#warning REMOVE delay in _fatal_common when uart_flush is fixed
	_delay_ms(50);
	
#if FATAL_BREAK
	BREAK();
#endif
	
#if FATAL_RESET
	reset();
#else
	die();
#endif
}


noreturn void fatal(const char *pstr) {
	_fatal_common(pstr, true);
}

noreturn void fatal_S(const char *str) {
	_fatal_common(str, false);
}
