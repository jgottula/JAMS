/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef JAMS_ALGO_FIFO_H
#define JAMS_ALGO_FIFO_H


#include "std.h"


struct fifo {
	uint8_t i_push;
	uint8_t i_pop;
	
	uint8_t size;
	uint8_t count;
	
	uint8_t *data;
};


void fifo_init(struct fifo *fifo, uint8_t size);
void fifo_deinit(struct fifo *fifo);

bool fifo_push(struct fifo *fifo, uint8_t val);
bool fifo_pop(struct fifo *fifo, uint8_t *out);

bool fifo_push16(struct fifo *fifo, uint16_t val);
bool fifo_pop16(struct fifo *fifo, uint16_t *out);

bool fifo_push_force(struct fifo *fifo, uint8_t val);
bool fifo_push16_force(struct fifo *fifo, uint16_t val);


#endif
