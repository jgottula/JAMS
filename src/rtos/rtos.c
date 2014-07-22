/* JAMS
 * (c) 2014 Justin Gottula
 * The source code of this project is distributed under the terms of the
 * simplified BSD license. See the LICENSE file for details.
 */


#include "rtos/rtos.h"
#include "task/task_a.h"
#include "task/task_b.h"


extern void __stack;
uintptr_t next_stack = (uintptr_t)&__stack;

uint8_t active_task = 0;
uint8_t task_count = 0;
struct task tasks[8];


ISR(RTC_OVF_vect, ISR_NAKED) {
	uint8_t old_spl, old_sph;
	uint8_t new_spl, new_sph;
	
	
	asm volatile (
		"\
		\n push r0 \
		\n in r0,__SREG__ \
		\n push r0 \
		\n in r0,0x3b \
		\n push r0 \
		\n in r0,0x3a \
		\n push r0 \
		\n in r0,0x39 \
		\n push r0 \
		\n in r0,0x38 \
		\n push r0 \
		\n push r31 \
		\n push r30 \
		\n push r29 \
		\n push r28 \
		\n push r27 \
		\n push r26 \
		\n push r25 \
		\n push r24 \
		\n push r23 \
		\n push r22 \
		\n push r21 \
		\n push r20 \
		\n push r19 \
		\n push r18 \
		\n push r17 \
		\n push r16 \
		\n push r15 \
		\n push r14 \
		\n push r13 \
		\n push r12 \
		\n push r11 \
		\n push r10 \
		\n push r9 \
		\n push r8 \
		\n push r7 \
		\n push r6 \
		\n push r5 \
		\n push r4 \
		\n push r3 \
		\n push r2 \
		\n push r1 \
		\n eor r1,r1 \
		"
		:
		:
		: "memory"
	);
	
	asm volatile (
		"\
		\n nop \
		\n in %[dst_lo],%[spl] \
		\n in %[dst_hi],%[sph] \
		"
		: [dst_lo] "=r" (old_spl),
		  [dst_hi] "=r" (old_sph)
		: [spl] "I" (AVR_STACK_POINTER_LO_ADDR),
		  [sph] "I" (AVR_STACK_POINTER_HI_ADDR)
		:
	);
	
	
	/* highly advanced scheduling algorithm */
	uint8_t next_tid;
	if (task_count == 1) {
		/* stay on the idle task */
		next_tid = active_task;
	} else {
		next_tid = active_task + 1;
		if (next_tid >= task_count) {
			next_tid = 1;
		}
	}
	
	
	struct task *old_task = tasks + active_task;
	struct task *new_task = tasks + next_tid;
	
	
	old_task->spl = old_spl;
	old_task->sph = old_sph;
	
	new_spl = new_task->spl;
	new_sph = new_task->sph;
	
	active_task = next_tid;
	
	
	asm volatile (
		"\
		\n out %[spl],%[src_lo] \
		\n out %[sph],%[src_hi] \
		\n nop \
		"
		:
		: [src_lo] "r" (new_spl),
		  [src_hi] "r" (new_sph),
		  [spl] "I" (AVR_STACK_POINTER_LO_ADDR),
		  [sph] "I" (AVR_STACK_POINTER_HI_ADDR)
		:
	);
	
	asm volatile (
		"\
		\n pop r1 \
		\n pop r2 \
		\n pop r3 \
		\n pop r4 \
		\n pop r5 \
		\n pop r6 \
		\n pop r7 \
		\n pop r8 \
		\n pop r9 \
		\n pop r10 \
		\n pop r11 \
		\n pop r12 \
		\n pop r13 \
		\n pop r14 \
		\n pop r15 \
		\n pop r16 \
		\n pop r17 \
		\n pop r18 \
		\n pop r19 \
		\n pop r20 \
		\n pop r21 \
		\n pop r22 \
		\n pop r23 \
		\n pop r24 \
		\n pop r25 \
		\n pop r26 \
		\n pop r27 \
		\n pop r28 \
		\n pop r29 \
		\n pop r30 \
		\n pop r31 \
		\n pop r0 \
		\n out 0x38,r0 \
		\n pop r0 \
		\n out 0x39,r0 \
		\n pop r0 \
		\n out 0x3a,r0 \
		\n pop r0 \
		\n out 0x3b,r0 \
		\n pop r0 \
		\n out __SREG__,r0 \
		\n pop r0 \
		\n reti \
		"
		:
		:
		: "memory"
	);
}


void rtos_init(void) {
	/* set up the idle task */
	active_task = TID_IDLE;
	task_count = 1;
	tasks[TID_IDLE].status = TST_RUNNABLE;
	tasks[TID_IDLE].sp = 0;
	
	#warning TODO: add a stack begin/end var to the task struct
	
	
	rtos_new_task(&task_a);
	rtos_new_task(&task_b);
	
	
	/* use 1.024kHz RTC clock source from the calibrated internal oscillator */
	//CLK.RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;
	CLK.RTCCTRL = CLK_RTCSRC_ULP_gc | CLK_RTCEN_bm;
	#warning if not using ULP, you may need to turn the oscillator on (OSC)
	
	/* reset the count and fire off an interrupt every 300 RTC ticks */
	RTC.PER = 300;
	
	/* we aren't using this */
	RTC.COMP = 0;
	
	/* reset the count to zero */
	RTC.CNT = 0;
	
	/* enable RTC overflow interrupt with low priority */
	RTC.INTCTRL = RTC_OVFINTLVL_LO_gc;
	
	/* enable the RTC with no clock prescaling */
	RTC.CTRL = RTC_PRESCALER_DIV1_gc;
	
	
	// return to main() which will call idle()
}


void rtos_new_task(uintptr_t begin) {
	// next_stack will initially contain the value 0x3fff
	// use this to set SP for new tasks
	next_stack -= TASK_STACK_SIZE;
	
	// preload the stack with zeroes for where the registers will be restored
	// and put 'begin' as the return address
	
	// in the future, put an additional return address at the bottom of the
	// stack so that if tasks return from the top-level function, it will return
	// to an exit_task() function instead of doing bad things
	
	struct task *this_task = tasks + task_count;
	
	this_task->status = TST_RUNNABLE;
	this_task->sp = next_stack - sizeof(struct task_context);
	
	struct task_context *ctx = (struct task_context *)(this_task->sp + 1);
	memset(ctx, 0x00, sizeof(*ctx));
	ctx->sreg  = 0x80;
	ctx->pc_hi = begin >> 8;
	ctx->pc_lo = begin;
	
	++task_count;
}
