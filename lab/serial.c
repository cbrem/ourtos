/* 
 * serial.c - A small serial library.
 *
 * Connor Brem (cbrem)
 * Spencer Barton (sebarton)
 * Group C1
 * 18-348 Lab 11
 */

#include "serial.h"

// TODO: instead of prescale, maybe pass in
// type which chooses baud from an enum?
void serialInit(baud_t baud) {
    uint16_t prescale;

    prescale = _prescaleForBaud(baud);

    // TODO: put these in macros?
    SCIBDL = prescale & 0xFF;
    SCIBDH = (prescale >> 8) & 0xFF;

    /* Configure for 8 bit, one stop bit, no parity. */
    SCICR1 = 0x00;
    SCICR2 = 0x00;
}

void serialWrite(byte_t buffer[], uint8_t len) {
    uint8_t i;

    for (i = 0; i < len; i++) {
        _serialWriteByte(buffer[i]);
    }
}

void serialRead(byte_t buffer[], uint8_t len) {
    uint8_t i;

    for (i = 0; i < len; i++) {
        buffer[i] = _serialReadByte();
    }
}

static byte_t _serialReadByte() {
  SCICR2_RE = 0x1;
  while(!SCISR1_RDRF);
  return SCIDRL;
}

static void _serialWriteByte(byte_t byte) {
    SCICR2_TE = 0x1;
    while (!SCISR1_TDRE);
    SCIDRL = byte;
}

static uint16_t _prescaleForBaud(baud_t baud) {
    switch(baud) { 
    case BAUD_300:   return 0x0683;
    case BAUD_600:   return 0x0341;
    case BAUD_1200:  return 0x01a1;
    case BAUD_2400:  return 0x00d0;
    case BAUD_4800:  return 0x0068;
    case BAUD_9600:  return 0x0034;
    case BAUD_14400: return 0x0023;
    case BAUD_19200: return 0x001a;
    case BAUD_38400: return 0x000d;
    case BAUD_56000: return 0x0009;
    case BAUD_115200: return 0x0004;
    }
}
