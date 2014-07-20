/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef JAMS_STD_H
#define JAMS_STD_H


/* standard library */
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdnoreturn.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* avr-libc specific */
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/atomic.h>
#include <util/delay.h>


/* project: common headers */
#include "macro.h"
#include "mcu/die.h"
#include "mcu/mem.h"
#include "mcu/reset.h"
#include "debug/assert.h"
#include "debug/break.h"
#include "debug/fatal.h"
#include "io/stdfile.h"


#endif
