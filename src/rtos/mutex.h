/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef JAMS_RTOS_MUTEX_H
#define JAMS_RTOS_MUTEX_H


#include "std.h"


typedef volatile uint8_t mutex;


void mutex_create(mutex *m);
void mutex_destroy(mutex *m);

void mutex_acquire(mutex *m);
void mutex_release(mutex *m);


#endif
