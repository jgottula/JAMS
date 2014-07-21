/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "task/task_a.h"


void task_a(void) {
	for ( ;; );
	
	// set the scheduler to run every 1000 ms
	// task A and B will both do the following:
	//
	// loop forever:
	//  acquire uart mutex
	//  for each character in 'Hello from Task A/B!\n':
	//   send the character
	//   _delay_ms(100)
	//  release uart mutex
	//  _delay_ms(1000)
}
