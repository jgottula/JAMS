/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef JAMS_DEBUG_BREAK_H
#define JAMS_DEBUG_BREAK_H


#include "std.h"


#define BREAK() \
	__asm__ __volatile__( \
		"\nbreak" \
		: \
		: \
		: "memory" \
	)


#endif
