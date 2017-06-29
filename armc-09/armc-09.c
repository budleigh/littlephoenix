/*

    Part of the Raspberry-Pi Bare Metal Tutorials
    Copyright (c) 2013, Brian Sidebotham
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
        this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
        this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

*/

/*
    C-Library stubs introduced for newlib
*/

#include <string.h>
#include <stdlib.h>

#include "rpi-GPIO.h"

/** GPIO Register set */
volatile unsigned int* gpio = (unsigned int*)GPIO_BASE;

/* 
// following cloned from RPi.GPIO-0.6.3 python GPIO lib

#define FSEL_OFFSET                 0   // 0x0000
#define SET_OFFSET                  7   // 0x001c / 4
#define CLR_OFFSET                  10  // 0x0028 / 4

#define INPUT  1 // is really 0 for control register!
#define OUTPUT 0 // is really 1 for control register!
#define ALT0   4

#define HIGH 1
#define LOW  0

void output_gpio(int gpio_chan, int value)
{
    int offset, shift;

    if (value) // value == HIGH
        offset = SET_OFFSET + (gpio_chan / 32);
    else       // value == LOW
       offset = CLR_OFFSET + (gpio_chan / 32);

    shift = gpio_chan % 32;
    gpio[offset] = 1 << shift;
}

void setup_gpio(int gpio_chan, int direction)
{
    int offset = FSEL_OFFSET + (gpio_chan / 10);
    int shift = (gpio_chan % 10) * 3;

    if (direction == OUTPUT)
        gpio[offset] = (gpio[offset] & ~(0x07 << shift)) | (0x01 << shift);
    else  // direction == INPUT
        gpio[offset] = (gpio[offset] & ~(0x07 << shift));
}

// following recoded from blinkt.py in blinkt_master lib

#define DAT 23  // blinkt data GPIO bit
#define CLK 24  // blinkt clock GPIO bit
#define NUM_PIXELS 8
#define BRIGHTNESS 7

typedef unsigned char uint8;

uint8 pixels[7][4];

void init_blinkt() {
	// initialize blinkt pixel array
	for (int i = 0; i < NUM_PIXELS; i++) 
		for (int j = 0; j < 4; j++) {
			pixels[i][j] = j < 3 ? 0 : BRIGHTNESS;  // r, g, b, brightness
	//
	setup_gpio(DAT, OUTPUT);
	setup_gpio(CLK, OUTPUT);
}

void clear() {
	int i, j;
	// set all pixel r,g,b values to 0
	for (int i = 0; i < NUM_PIXELS; i++) 
		for (int j = 0; j < 3; j++) {
			pixels[i][j] = 0;
		}
	}
}

void _write_byte(uint8 byte) {
	// clocks out the byte value one bit at a time
	for (int i = 0; i < 8; i++) {
		output_gpio(DAT, byte & 0x80);
		output_gpio(CLK, 1);
		byte <<= 1;
		output_gpio(CLK, 0);
	}
}

// Emit exactly enough clock pulses to latch the small dark die APA102s which are weird
// for some reason it takes 36 clocks, the other IC takes just 4 (number of pixels/2)
void _eof() {
    output_gpio(DAT, 0);
    for (int i = 0; i < 36; i++); {
        output_gpio(CLK, 1);
        output_gpio(CLK, 0);
    }
}

void _sof() {
    output_gpio(DAT, 0);
    for (int i = 0; i < 32; i++) {
        output_gpio(CLK, 1);
        output_gpio(CLK, 0);
    }
}

void show() {
    // Output the buffer to Blinkt
    _sof();
	for (int i = 0; i < NUM_PIXELS; i++) {
        _write_byte(0xe0 | pixels[i][3]); // brightness
        _write_byte(pixels[i][2]);  // blue
        _write_byte(pixels[i][1]);  // green
        _write_byte(pixels[i][0]);  // red
    }
    _eof();
}

void set_pixel(uint8 pixel, uint8 r, uint8 g, uint8 b, float brightness) {
    // set the RGB value, and optionally brightness, of a single pixel
	// brightness 0.0 to 1.0, normal 0.2
	pixels[pixel][3] = (int)(31.0 * brightness) & 0x1f;
	pixels[pixel][0] = r;
	pixels[pixel][1] = g;
	pixels[pixel][2] = b;
}

void set_all(uint8 r, uint8 g, uint8 b, float brightness) {
    // Set the RGB value and optionally brightness of all pixels
    for (int i = 0; i < NUM_PIXELS; i++) 
    	set_pixel(i, r, g, b, brightness);
}

// following recoded from solid_colors.py example in blinkt_master

void solid_colors() {
	int step = 0;
	while (1) {
		switch (step) {
			case 0:
				set_all(128, 0, 0, 0.4);
				break;
			case 1:
				set_all(0, 128, 0, 0.4);
				break;
			case 2:
				set_all(0, 0, 128, 0.4);
				break;
		}
		
		step += 1;
		step %= 3;
		show();
		// wait a bit
		for (int i = 0; i < 500000; i++)
			;
	}
}

*/

/** Main function - we'll never return from here */
void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags)
{
	//init_blinkt();
	//solid_colors();


    int loop;
    unsigned int* counters;

    /* Set the LED GPIO pin to an output to drive the LED */
    gpio[LED_GPFSEL] |= ( 1 << LED_GPFBIT );

    /* Allocate a block of memory for counters */
    counters = malloc( 1024 * sizeof( unsigned int ) );

    /* Failed to allocate memory! */
    if( counters == NULL )
        while(1) {     LED_ON();/* Trap here */ }

    for( loop=0; loop<1024; loop++ )
        counters[loop] = 0;

    /* Never exit as there is no OS to exit to! */
    while(1)
    {
        /* Light the LED */
        LED_ON();

        for(counters[0] = 0; counters[0] < 500000; counters[0]++)
            ;

        /* Set the GPIO16 output low ( Turn OK LED on )*/
        LED_OFF();

        for(counters[1] = 0; counters[1] < 500000; counters[1]++)
            ;
    }
}
