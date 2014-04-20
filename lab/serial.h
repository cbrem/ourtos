/* 
 * serial.h - A small serial library.
 */

#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdint.h>

/*
 * Configures serial communications, using the given baud prescale.
 */
void serialInit(uint16_t prescale);

/*
 * Writes len bytes of the given buffer over serial.
 */
void serialWrite(char[] buffer, uint8_t len);

/*
 * Reads len bytes from serial into the given buffer.
 */
void serialRead(char[] buffer, uint8_t len);

#endif // _SERIAL_H
