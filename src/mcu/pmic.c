/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "mcu/pmic.h"


void pmic_init(void) {
	/* enable round-robin scheduling and enable all priority levels */
	PMIC.CTRL = PMIC_RREN_bm |
		PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
	
	/* turn the global interrupt flag on */
	sei();
}
