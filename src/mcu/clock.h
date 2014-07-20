/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#ifndef JAMS_MCU_CLOCK_H
#define JAMS_MCU_CLOCK_H


#include "std.h"


void mcu_setup_xtal(void);
void mcu_setup_pll(void);

void mcu_use_xtal(void);
void mcu_use_pll(void);


#endif
