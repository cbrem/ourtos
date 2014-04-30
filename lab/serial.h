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
 * Configures serial communications, using the given baud rate.
 */
void serialInit(uint16_t baud);

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

#endif // _SERIAL_H
