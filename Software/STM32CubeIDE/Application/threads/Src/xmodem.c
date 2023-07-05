/*
 * xmodem_global_handle.h
 *
 *  Created on: Jul 4, 2023
 *      Author: calebdavis
 */

#include <string.h>
#include "xmodem.h"
#include "stm32f4xx.h"

/* Protocol control ASCII characters */
#define SOH                       0x01
#define STX                       0x02
#define EOT                       0x04
#define ACK                       0x06
#define NAK                       0x15
#define SUB                       0x1A
#define POLL                      0x43
#define POLL_PERIOD_ms            1000
#define PACKET_TIMEOUT_PERIOD_ms  7000
#define LONG_PACKET_PAYLOAD_LEN   1024
#define PACKET_HEADER_LEN         3

static uint8_t __rxBuffer[1029];

typedef enum
{
  XMODEM_STANDARD,
  XMODEM_1K
} eXModemPacketType_t;

typedef struct
{
  XModem1KPayloadCallback callback;
  XModem1KWriteMethod     write;
  XModem1KReadMethod      read;
  XModem1KTimerStart      timer_start;
  XModem1KTimerKill       timer_kill;
  XModem1KTimerExpired    timer_expired;
  uint16_t                byte_count;     // Byte being operated on in packet
  uint16_t                crc;
  uint8_t                 in_progress;    // Indicates actively receiving a file
  uint8_t                 data;           // Last received byte
  uint8_t                 data_started;   // Indicates transfer in progress
  uint8_t                 block_number;   // Current block number being transfered
  eXModemPacketType_t     packet_type;
  uint8_t                 buffer[LONG_PACKET_PAYLOAD_LEN] __attribute__ ((aligned(4)));
} xXModem1KOperatingStruct_t;

static xXModem1KOperatingStruct_t OperatingParams[XMODEM1KNUMBEROFOBJECTS];
static uint8_t                    OperatingParamsPosition = 0;

static uint16_t __CRCCalc( const uint8_t *pu8Data, int16_t i16Len )
{
  uint8_t i;
  uint16_t u16CRC = 0;

  while( --i16Len >= 0 )
  {
    i = 8;
    u16CRC = u16CRC ^ (((uint16_t) *pu8Data++) << 8);

    do
    {
      if( u16CRC & 0x8000 )
      {
        u16CRC = u16CRC << 1 ^ 0x1021;
      }
      else
      {
        u16CRC = u16CRC << 1;
      }
    } while( --i );
  }
  return u16CRC;
}

uint8_t __CSMCalc( const uint8_t *pu8Data, uint16_t u16Length )
{
  uint8_t u8CheckSum=0;
  uint16_t u16Position=0;

  for( u16Position=0;u16Position<u16Length;u16Position++ )
  {
    u8CheckSum += pu8Data[u16Position];
  }
  return u8CheckSum;
}

static uint8_t __WaitForReceiveToken( xXModem1KOperatingStruct_t * xmodem1k_handle,
                                      uint8_t                      token,
                                      uint32_t                     max_wait_time )
{
  xmodem1k_handle->timer_start( max_wait_time );

  do
  {
    while( 0 == xmodem1k_handle->read( &xmodem1k_handle->data ) )
    {
      if( xmodem1k_handle->timer_expired() )
      {
        /* returns '0' when the token is not received and the time out occurs */
        return 0;
      }
    }
  } while( xmodem1k_handle->data != token );

  xmodem1k_handle->timer_kill();

  /* returns '1' when token is received */
  return 1;
}

XModem1KHandle XModem1KCreate( XModem1KWriteMethod  write,
                               XModem1KReadMethod   read,
                               XModem1KTimerStart   timer_start,
                               XModem1KTimerKill    timer_kill,
                               XModem1KTimerExpired timer_expired  )
{
  xXModem1KOperatingStruct_t * new_object = (void*)0;

  if( OperatingParamsPosition < XMODEM1KNUMBEROFOBJECTS )
  {
    new_object = (XModem1KHandle)(&OperatingParams[OperatingParamsPosition++]);
  }
  else
  {
    for( ;; )
    {
      // If you are here, you have allocated too many XModem1k Objects
      // Try increasing XMODEM1KNUMBEROFOBJECTS until you are not here
      new_object++;
    }

    new_object = (XModem1KHandle)(&OperatingParams[2]);
  }

  new_object->write         = write;
  new_object->read          = read;
  new_object->timer_start   = timer_start;
  new_object->timer_kill    = timer_kill;
  new_object->timer_expired = timer_expired;

  return (XModem1KHandle)new_object;
}

void XModem1KReceiveFileInit( XModem1KHandle handle )
{
  xXModem1KOperatingStruct_t * xmodem1k_handle = (xXModem1KOperatingStruct_t*)handle;

  xmodem1k_handle->byte_count   = 0;
  xmodem1k_handle->crc          = 0;
  xmodem1k_handle->data         = 0;
  xmodem1k_handle->data_started = 0;
}


// TODO: Clean up the byte numbering.
uint8_t XModem1KReceiveFile( XModem1KHandle          handle,
                             XModem1KPayloadCallback payload )
{
  xXModem1KOperatingStruct_t * xmodem1k_handle = (xXModem1KOperatingStruct_t*)handle;

  XModem1KReceiveFileInit( handle );

  xmodem1k_handle->in_progress = 1;

  /* Send command to server indicating we are ready to receive */
  xmodem1k_handle->write( POLL );
  xmodem1k_handle->timer_start( POLL_PERIOD_ms );


  while( xmodem1k_handle->in_progress )
  {
    /* Check if a character has been received on the UART */
//    if( xmodem1k_handle->read( &xmodem1k_handle->data ) )

//	  uint8_t i = 0;
//	  uint8_t byte = 0;
//	  uint8_t __bufPos = 0;
//	  HAL_StatusTypeDef status;
//
//	while( __bufPos < 1029 )
//	{
//	    xmodem1k_handle->timer_kill();
//	    xmodem1k_handle->data_started = 1;
//		status = xmodem1k_handle->read( &byte );
//		if( status == 1 )
//		{
//			__rxBuffer[__bufPos] = byte;
//			__bufPos++;
//		}
//
//		if( __bufPos == 1028 )
//		{
//			while(1);
//		}
//
//	    if( __bufPos == 0 )
//	    {
//	      // Writes a 'C' to file sender to trigger a write if available
//	      if( 1 || xmodem1k_handle->timer_expired() && ( 0 == xmodem1k_handle->data_started  ) )
//	      {
//	        xmodem1k_handle->write( POLL );
//	        xmodem1k_handle->timer_start( POLL_PERIOD_ms );
//	      }
//	    }
//	}
	if( xmodem1k_handle->read( &xmodem1k_handle->data ) )
    {
      /* Position of received byte determines action we take */
      xmodem1k_handle->timer_kill();
      xmodem1k_handle->data_started = 1;

      switch( xmodem1k_handle->byte_count )
      {
        case 0:
          if( xmodem1k_handle->data == STX )
          {
            xmodem1k_handle->packet_type   = XMODEM_1K;
            xmodem1k_handle->byte_count = 1;
          }
          else if( xmodem1k_handle->data == SOH )
          {
            xmodem1k_handle->packet_type   = XMODEM_STANDARD;
            xmodem1k_handle->byte_count = 1;
          }
          else if( xmodem1k_handle->data == EOT || xmodem1k_handle->data == SUB )
          {
            xmodem1k_handle->write( ACK );
            xmodem1k_handle->in_progress = 0;
          }
          else
          {
            while( 1 );
          }
          break;
        case 1:
          /* Byte 1 is the packet number - should be different from last one we received */
        case 2:
          /* Byte 2 is the packet number inverted - check for error with last byte */
          xmodem1k_handle->byte_count++;
          break;
        case 127:
          if( XMODEM_STANDARD == xmodem1k_handle->packet_type )
          {
            xmodem1k_handle->crc = xmodem1k_handle->data;
          }
          else
          {
            // Stuff byte into received array
            xmodem1k_handle->buffer[xmodem1k_handle->byte_count - PACKET_HEADER_LEN] =
                xmodem1k_handle->data;
          }
          xmodem1k_handle->byte_count++;
          break;
        case 128:
          if( XMODEM_STANDARD == xmodem1k_handle->packet_type )
          {
            xmodem1k_handle->crc <<= 8;
            xmodem1k_handle->crc  |= xmodem1k_handle->data;

            //if( __CRCCalc( xmodem1k_handle->buffer, 128 ) == xmodem1k_handle->crc )
            if( 1 )
            {
              if( payload( xmodem1k_handle->buffer ) != 0 )
              {
                xmodem1k_handle->write( ACK );
              }
              else
              {
                xmodem1k_handle->write( NAK );
              }
            }
            else /* Error CRC calculated does not match that received */
            {
              xmodem1k_handle->write( NAK );
            }

            xmodem1k_handle->byte_count = 0;
          }
          else
          {
            // Stuff byte into received array
            xmodem1k_handle->buffer[xmodem1k_handle->byte_count - PACKET_HEADER_LEN] =
                xmodem1k_handle->data;
            xmodem1k_handle->byte_count++;
          }
          break;
        case 1027:
          xmodem1k_handle->crc = xmodem1k_handle->data;
          xmodem1k_handle->byte_count++;
          break;
        case 1028:
          xmodem1k_handle->crc <<= 8;
          xmodem1k_handle->crc  |= xmodem1k_handle->data;

          if( __CRCCalc( xmodem1k_handle->buffer, 1024 ) == xmodem1k_handle->crc )
          {
            if( payload( xmodem1k_handle->buffer ) != 0 )
            {
              xmodem1k_handle->write( ACK );
            }
            else
            {
              xmodem1k_handle->write( NAK );
            }
          }
          else /* Error CRC calculated does not match that received */
          {
            xmodem1k_handle->write( NAK );
          }

          xmodem1k_handle->byte_count = 0;

          break;
        default:
          // Stuff byte into received array
          xmodem1k_handle->buffer[xmodem1k_handle->byte_count - PACKET_HEADER_LEN] =
              xmodem1k_handle->data;
          xmodem1k_handle->byte_count++;
          break;
      }
    }
    else
    {
      // Writes a 'C' to file sender to trigger a write if available
      if( xmodem1k_handle->timer_expired() && ( 0 == xmodem1k_handle->data_started  ) )
      {
        xmodem1k_handle->write( POLL );
        xmodem1k_handle->timer_start( POLL_PERIOD_ms );
      }
    }
  }

  // TODO: Implement failure modes
  return 1;
}

uint8_t XModem1KSendFileInit( XModem1KHandle handle )
{
  // Cast object to struct for easy code handling and readability
  xXModem1KOperatingStruct_t * xmodem1k_handle = (xXModem1KOperatingStruct_t*)handle;

  xmodem1k_handle->block_number = 01;


  // TODO: Danfoss Does not Send 'C', so the application will halt here.
  // TODO: Loop until 'C' is seen from target, for a maximum amount of time
  //xmodem1k_handle->timer_start( 15000 );
  //return __WaitForReceiveToken( xmodem1k_handle, 'C', 5000 );
  return 1;
}

// TODO: Return number of bytes written
uint8_t XModem1KSendPacket( XModem1KHandle handle,
                            uint8_t      * packet,
                            uint16_t       length,
                            uint8_t        useCRC  )
{
  // Cast object to struct for easy code handling and readability
   xXModem1KOperatingStruct_t * xmodem1k_handle = (xXModem1KOperatingStruct_t*)handle;

   xmodem1k_handle->byte_count = 0;
   xmodem1k_handle->block_number;

   xmodem1k_handle->write( STX ); // XModem1K Header

   // Block Number and Inverted Block Number
   xmodem1k_handle->write(  xmodem1k_handle->block_number );
   xmodem1k_handle->write( ~xmodem1k_handle->block_number );

   if( xmodem1k_handle->buffer != packet )
   {
     memset( xmodem1k_handle->buffer, 0, LONG_PACKET_PAYLOAD_LEN );
     memcpy( xmodem1k_handle->buffer, packet, length );
   }

   // Send 1K Buffer
   while( xmodem1k_handle->byte_count < LONG_PACKET_PAYLOAD_LEN )
   {
     // TODO: Add error handling or timeouts here
     xmodem1k_handle->write( xmodem1k_handle->buffer[xmodem1k_handle->byte_count++] );
   }

   if( useCRC )
   {
     xmodem1k_handle->crc = __CRCCalc( xmodem1k_handle->buffer, LONG_PACKET_PAYLOAD_LEN );

     // Send CRC
     xmodem1k_handle->write( (uint8_t)(( xmodem1k_handle->crc>>8)&0xFF)); // Upper Byte
     xmodem1k_handle->write( (uint8_t)(( xmodem1k_handle->crc   )&0xFF)); // Lower Byte
   }
   else
   {
    xmodem1k_handle->write(__CSMCalc( xmodem1k_handle->buffer, LONG_PACKET_PAYLOAD_LEN ));
   }

   if( __WaitForReceiveToken( xmodem1k_handle, ACK, 3000 ) )
   {
     xmodem1k_handle->block_number++;
     return 1;
   }

   return 0;
}

void XModem1KSendEndTransfer( XModem1KHandle handle )
{
  xXModem1KOperatingStruct_t * xmodem1k_handle = (xXModem1KOperatingStruct_t*)handle;
  xmodem1k_handle->write( EOT );
}

uint8_t * XModem1KGetSendBuffer( XModem1KHandle handle )
{
  xXModem1KOperatingStruct_t * xmodem1k_handle = (xXModem1KOperatingStruct_t*)handle;

  return xmodem1k_handle->buffer;
}
