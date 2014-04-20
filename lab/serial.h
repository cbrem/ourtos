/* 
 * serial.h
 * 
 * A small serial library.
 */

#IFNDEF _SERIAL_H
#DEFINE _SERIAL_H

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

#ENDIF // _SERIAL_H
