/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "mcu/clock.h"


ISR(OSC_OSCF_vect) {
	/* oscillator failure */
	
	for (;;);
}


void mcu_setup_xtal(void) {
	/* use external 16MHz crystal */
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
	OSC.CTRL |= OSC_XOSCEN_bm;
	
	/* wait until the clock is stable */
	while (!(OSC.STATUS & OSC_XOSCRDY_bm));
}

void mcu_setup_pll(void) {
	/* enable PLL with 2x multiplication factor */
	OSC.PLLCTRL = OSC_PLLSRC_XOSC_gc | 0x2;
	OSC.CTRL |= OSC_PLLEN_bm;
	
	/* wait until PLL output is stable */
	while (!(OSC.STATUS & OSC_PLLRDY_bm));
}


void mcu_use_xtal(void) {
	/* use external crystal as system clock */
	CPU_CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_XOSC_gc;
}

void mcu_use_pll(void) {
	/* use PLL output as system clock */
	CPU_CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_PLL_gc;
}
