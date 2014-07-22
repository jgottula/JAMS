/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "task/task_a.h"
#include "dev/uart/uart.h"
#include "rtos/mutex.h"


extern mutex test_mutex;


void task_a(void) {
	const char s[] = "AAAaaa";
	
	for ( ;; ) {
		mutex_acquire(&test_mutex);
		
		const char *p = s;
		while (*p != '\0') {
			uart_write(*(p++));
			_delay_ms(100);
		}
		
		mutex_release(&test_mutex);
		
		_delay_ms(300);
	}
}
