/*
 * intTypes.h - a few macros for more explicit types, 
 *  included because stdint not available
 *  WARNING this is chip dependent - we are using MC9S12C128
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
typedef unsigned char ubyte_t;

#define WORD_LEN_BITS (8)
#define BYTE_LEN_BITS (4)

#endif // _INTTYPES_MC9S12C128_H