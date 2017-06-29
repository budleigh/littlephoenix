/******************************************************************************
*	main.s
*	 by Alex Chadwick
*
*	A sample assembly code implementation of the ok02 operating system, that 
*	simply turns the OK LED on and off repeatedly.
*	Changes since OK01 are marked with NEW.
******************************************************************************/

/*
* .section is a directive to our assembler telling it to place this code first.
* .globl is a directive to our assembler, that tells it to export this symbol
* to the elf file. Convention dictates that the symbol _start is used for the 
* entry point, so this all has the net effect of setting the entry point here.
* Ultimately, this is useless as the elf itself is not used in the final 
* result, and so the entry point really doesn't matter, but it aids clarity,
* allows simulators to run the elf, and also stops us getting a linker warning
* about having no entry point. 
*/
.section .init
.globl _start
_start:

/* JBW: note changes for RPi2: GPIO base address 0x3F200000, OK light GPIO bit 47

/* 
* This command loads the physical address of the GPIO region into r0.
*/ 
ldr r0,=0x3F200000
  
/*
* Our register use is as follows:
* r0=0x3F200000	the address of the GPIO region.   
* r1=0x00040000	a number with bits 21-23 set to 001 to put into the GPIO
*				function select to enable output to GPIO 47. 
* then
* r1=0x00010000	a number with bit 15 high, so we can communicate with GPIO 47.
* r2=0x003F0000 a number that will take a noticeable duration for the processor 
*				to decrement to 0, allowing us to create a delay.
*/
mov r1,#1
lsl r1,#21   

/*
* Set the GPIO function select.  FSEL register 4, so offset is 4 * 4 bytes per reg = 16
*/
str r1,[r0,#16]

/* 
* Set the 15th bit of r1.  Corresponds to bit 47, the 15 bit in the CLR1 register
*/
mov r1,#1
lsl r1,#15

/* NEW
* Label the next line loop$ for the infinite looping
*/
loop$: 

/*
* Set GPIO 47 to low, causing the LED to turn on.  CLR1 = reg 11 so offset 11 * 4 = 44
*/
str r1,[r0,#44]

/* NEW
* Now, to create a delay, we busy the processor on a pointless quest to 
* decrement the number 0x3F0000 to 0!
*/
mov r2,#0x3F0000
wait1$:
	sub r2,#1
	cmp r2,#0
	bne wait1$

/* NEW
* Set GPIO 16 to high, causing the LED to turn off.  SET1 = reg 8 so offset = 8 * 4 = 32
*/
str r1,[r0,#32]

/* NEW
* Wait once more.
*/
mov r2,#0x3F0000
wait2$:
	sub r2,#1
	cmp r2,#0
	bne wait2$

/*
* Loop over this process forevermore
*/
b loop$
