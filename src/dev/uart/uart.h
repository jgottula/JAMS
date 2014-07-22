/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef JAMS_DEV_UART_UART_H
#define JAMS_DEV_UART_UART_H


#include "std.h"


void uart_init(void);

void uart_flush(void);

uint8_t uart_avail(void);

bool uart_read(char *chr);
bool uart_write(char chr);


#endif
