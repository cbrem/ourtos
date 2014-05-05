/* 
 * serial.h - A small serial library.
 *
 * Connor Brem (cbrem)
 * Spencer Barton (sebarton)
 * Group C1
 * 18-348 Lab 11
 */

#ifndef _SERIAL_H
#define _SERIAL_H

/*==================================
 * Includes
 *==================================*/

#include <hidef.h>  
#include "derivative.h"
#include "inttypes_MC9S12C128.h"

/*
 * Baud rates which this serial library supports.
 */
// TODO: section for enums?
typedef enum {
    BAUD_300,
    BAUD_600,
    BAUD_2400,
    BAUD_4800,
    BAUD_9600,
    BAUD_14400,
    BAUD_19200,
    BAUD_38400,
    BAUD_56000,
    BAUD_115200
} baud_t;

/*==================================
 * MACROS
 *==================================*/

/*==================================
 * Exernal Globals
 *==================================*/

/*==================================
 * Local Globals
 *==================================*/

/*==================================
 * Public Functions
 *==================================*/

/*
 * Configures serial communications, using the given baud prescale.
 */
void serialInit(baud_t baud);

/*
 * Writes len bytes of the given buffer over serial.
 */
void serialWrite(char buffer[], uint8_t len);

/*
 * Reads len bytes from serial into the given buffer.
 */
void serialRead(char buffer[], uint8_t len);

/*==================================
 * Private Functions
 *==================================*/

/*
 * Reads one byte from serial.
 * Blocks until the byte arrives.
 */
 byte_t _serialReadByte();

/*
 * Writes one byte over serial.
 * Before writing, waits for serial buffer to clear.
 * Makes no guarantees about the state of the buffer after writing.
 */
void _serialWriteByte(char byte);

/*
 * Returns the prescale value which will yield the given baud.
 */
uint16_t _prescaleForBaud(baud_t baud);

#endif // _SERIAL_H
