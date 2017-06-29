#!/bin/sh

arm-none-eabi-gcc -O0 -DRPI2 -mfpu=neon-fp-armv8 -march=armv8-a -mtune=cortex-a5 -nostartfiles -g -Wl,-T,rpi.x armc-09.c armc-09-cstubs.c armc-09-cstartup.c armc-09-start.S -o kernel.elf
arm-none-eabi-objcopy kernel.elf -O binary kernel.img
