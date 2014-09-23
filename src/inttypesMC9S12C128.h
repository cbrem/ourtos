/*
 * intTypes.h - A few macros for more explicit types, 
 *  included because stdint not available.
 *  WARNING: These are chip/compiler dependent.
 *  We are using a Motorola MC9S12C128 microcontroller with the Freescale
 *  CodeWarrior S12(X) compiler.
 */

#ifndef _INTTYPES_MC9S12C128_H
#define _INTTYPES_MC9S12C128_H

/* int types are by number of bits */
typedef   signed char  int8_t;
typedef unsigned char uint8_t; 
typedef   signed int   int16_t;
typedef unsigned int  uint16_t;
typedef   signed long  int32_t;
typedef unsigned long uint32_t;

typedef char byte_t;
typedef int word_t;

#define WORD_LEN_BITS (16)
#define BYTE_LEN_BITS (8)
#define NIB_LEN_BITS (4)

#endif // _INTTYPES_MC9S12C128_H

