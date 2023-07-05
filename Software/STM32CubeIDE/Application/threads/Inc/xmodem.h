/*
 * xmodem_global_handle.h
 *
 *  Created on: Jul 4, 2023
 *      Author: calebdavis
 */

#ifndef XMODEM1K_H_
#define XMODEM1K_H_

#include <stdio.h>
#include <stdint.h>

#ifndef XMODEM1KNUMBEROFOBJECTS
#define XMODEM1KNUMBEROFOBJECTS 2
#endif

/**
 * Object handle when XModem1K Object is created.
 */
typedef void * XModem1KHandle;

/**
 * Callback method called when XModem1K has received a whole packet. It
 * is up to the user to determine what to do with the data in the defined
 * method.
 * @param payload
 * Pointer, from internal to XModem1K, that contains a received packet.
 * @param length
 * Length of amount of data contained in the payload.
 * @return
 * 1 Success, 0 failed to process data
 */
typedef uint8_t (*XModem1KPayloadCallback)( uint8_t * payload );

/**
 * Method used to write a byte to the connected XModem1K device. It is up to
 * the user to determine what to do when writing to the XModem1K device.
 * @param byte
 * Byte to be written to connected XModem1K device.
 */
typedef void (*XModem1KWriteMethod)( uint8_t byte );

/**
 * Method used to read data from the connected XModem1K device. This method
 * requires the user to detect if a byte is present to be read. If no byte is
 * to be read, the method returns immediately. It does not block on byte no
 * available.
 * @param byte
 * Method shall populate this referenced uint8_t with any available data, 1
 * byte only, from the connected XModem1K device.
 * @return
 * 1 if byte read, 0 if no byte was available.
 */
typedef uint8_t (*XModem1KReadMethod)( uint8_t * byte );

/**
 * Method used to start a timer. The actual implementation of the timing
 * function is up the user. This should be implemented as a one shot timer.
 * @param ms_timeout
 * Length of the timer in milliseconds.
 */
typedef void (*XModem1KTimerStart)( uint32_t ms_timeout );

/**
 * Method used to stop a timer from running.
 */
typedef void (*XModem1KTimerKill)( void );

/**
 * Method used to determine if a the timer, if started, has expired.
 * @return
 * 1 if timer has expired, 0 if the timer is still running.
 */
typedef uint8_t (*XModem1KTimerExpired)( void );

/**
 * Creates an XModem1K Object. Must be called before other XModem1K methods
 * can be called. All method definitions are defined in their type definitions.
 * @param write
 * @param read
 * @param timer_start
 * @param timer_kill
 * @param timer_expired
 * @return
 * Reference to new object.
 */
XModem1KHandle XModem1KCreate( XModem1KWriteMethod  write,
                               XModem1KReadMethod   read,
                               XModem1KTimerStart   timer_start,
                               XModem1KTimerKill    timer_kill,
                               XModem1KTimerExpired timer_expired );

/**
 * Call to initialize object to send file to connected XModem1K device.
 * @param handle
 */
void XModem1KReceiveFileInit( XModem1KHandle handle );

/**
 * Call this method to receive a whole file. The payload callback will
 * be called every time a complete packet is received.
 * @param handle
 * XModem1K object.
 * @param payload
 * Method called everytime a complete packet is received.
 * @return
 */
uint8_t XModem1KReceiveFile( XModem1KHandle          handle,
                             XModem1KPayloadCallback payload );

/**
 * Call to initialize object to send file to connected XModem1K device.
 * This method will initialize internal variables, and also wait on a
 * device to connect with, by finding a 'C' be written from the connected
 * device.
 * @param handle
 * XModem1K object.
 * @return
 * 1 if connected device is ready to receive, 0 if a timeout has occurred
 * without finding a connected device.
 */
uint8_t XModem1KSendFileInit( XModem1KHandle handle );

/**
 * Send single packet to connect XModem1K device. T
 * @param handle
 * @param packet
 * Pointer to array of bytes, between 1-1024 in length.
 * @param length
 * Length of array of bytes being pointed to.
 * @param useCRC
 * If 1, 16bit CRC will be Used.
 * @return
 */
uint8_t XModem1KSendPacket( XModem1KHandle handle,
                            uint8_t      * packet,
                            uint16_t       length,
                            uint8_t        useCRC  );

/**
 * Closes the XModem1K connection with the connected XModem1K device.
 * @param handle
 */
void XModem1KSendEndTransfer( XModem1KHandle handle );

/**
 * Returns the internal buffer used to send data over XModem1K. May be
 * used to lower RAM usage. Will need to understand the alignment needs
 * used by the buffer and transmitting mechanism.
 * @param handle
 * @return Pointer to 1024 byte buffer.
 */
uint8_t * XModem1KGetSendBuffer( XModem1KHandle handle );

#endif /* XMODEM1K_H_ */
