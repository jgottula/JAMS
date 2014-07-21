/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef JAMS_RTOS_RTOS_H
#define JAMS_RTOS_RTOS_H


#include "std.h"


enum {
	TST_RUNNABLE = 0,
	TST_BLOCKED  = 1,
	TST_IDLE     = 2,
	
	TID_IDLE = 0,
	
	TASK_STACK_SIZE  = 0x80,
};


struct task {
	uint8_t status;
	
	union {
		uint16_t sp;
		struct {
			uint8_t spl;
			uint8_t sph;
		};
	};
};


extern uint8_t active_task;
extern uint8_t task_count;
extern struct task tasks[8];


void rtos_init(void);

void rtos_new_task(uintptr_t begin);


#endif
