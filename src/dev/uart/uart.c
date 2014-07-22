/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "dev/uart/uart.h"
#include "algo/fifo.h"


static struct fifo fifo_rx;
static struct fifo fifo_tx;

static volatile struct {
	uint8_t init         : 1;
	uint8_t tx_active    : 1;
	uint8_t rx_overrun   : 1;
	uint8_t rx_fifo_full : 1;
} status;


ISR(USARTF0_RXC_vect) {
	if (USARTF0.STATUS & USART_BUFOVF_bm) {
		status.rx_overrun = true;
	}
	
	uint8_t byte = USARTF0.DATA;
	if (!fifo_push(&fifo_rx, byte)) {
		status.rx_fifo_full = true;
	}
}

ISR(USARTF0_DRE_vect) {
	/* last byte: disable future DRE interrupts */
	if (fifo_tx.count == 1) {
		status.tx_active = false;
		USARTF0.CTRLA &= ~USART_DREINTLVL_LO_gc;
	}
	
	uint8_t byte;
	if (fifo_pop(&fifo_tx, &byte)) {
		while (!(USARTF0.STATUS & USART_DREIF_bm));
		USARTF0.DATA = byte;
	}
}


void uart_init(void) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		/* 64 characters ought to be enough for anyone */
		fifo_init(&fifo_tx, 64);
		fifo_init(&fifo_rx, 64);
		
		/* set our TX high; also set FTDI's CTS# permanently low */
		PORTF.OUTSET = 0b00001000;
		PORTF.DIRSET = 0b00101000;
		
		/* use 9600 baud for now */
		uint16_t bsel   = 12;
		uint8_t  bscale = 4;
		USARTF0.BAUDCTRLA = bsel & 0xff;
		USARTF0.BAUDCTRLB = ((bsel >> 8) & 0x0f) | (bscale << 4);
		
		/* UART with no parity, one stop bit, 8-bit word size (8N1) */
		USARTF0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc |
			USART_CHSIZE_8BIT_gc;
		/* enable receipt and transmission */
		USARTF0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
		/* use high RXC priority to prevent overruns */
		USARTF0.CTRLA = USART_RXCINTLVL_HI_gc;
	}
	
	status.init = true;
}


void uart_flush(void) {
	if (!status.init) {
		return;
	}
	
	// TODO: flush MANUALLY in the future so that uart_flush can be called even
	// if interrupts are disabled (adjust die/reset/assert accordingly)
	for (bool done = false; !done; ) {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			done = (fifo_tx.count == 0);
		}
		
		if (!done) {
			_delay_us(1);
		}
	}
	
#warning TODO: wait for TX done int flag before returning
}


uint8_t uart_avail(void) {
	if (status.init) {
		return fifo_rx.count;
	} else {
		return 0;
	}
}


bool uart_read(char *chr) {
	bool result = false;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (status.init) {
			result = fifo_pop(&fifo_rx, (uint8_t *)chr);
		}
	}
	
	return result;
}

bool uart_write(char chr) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (status.init) {
			/* wait until tx fifo has room */
			/*while (!*/fifo_push(&fifo_tx, chr)/*) {
				NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE) {
					_delay_us(1)*/;/*
				}
			}*/
			
			/* first byte: enable DRE interrupts */
			if (!status.tx_active) {
				status.tx_active = true;
				USARTF0.CTRLA |= USART_DREINTLVL_LO_gc;
			}
		}
	}
	
	return true;
}
