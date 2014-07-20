/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef JAMS_MCU_MEM_H
#define JAMS_MCU_MEM_H


#include "std.h"


void *safe_malloc(size_t size) __attribute__((__malloc__));
#define malloc safe_malloc

void *safe_calloc(size_t nele, size_t size) __attribute__((__malloc__));
#define calloc safe_calloc


#endif
