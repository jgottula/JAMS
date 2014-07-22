/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "rtos/mutex.h"


void mutex_create(mutex *m) {
	*m = 0;
	
	/* no registration for now */
}

void mutex_destroy(mutex *m) {
	/* no de-registration for now */
}


void mutex_acquire(mutex *m) {
	bool retry;
	do {
		retry = false;
		
		/* spin lock */
		while (*m != 0);
		
		/* verify and acquire atomically */
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			if (*m == 0) {
				*m = 1;
			} else {
				retry = true;
			}
		}
	} while (retry);
}

void mutex_release(mutex *m) {
	*m = 0;
}
