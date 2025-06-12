/**
 * @file                                                      nfc_tag2_hal.c
 *
 * @cond INTERNAL_DOC
 * @brief                          NFC tag 2
 *
 * Version : 0.0.1
 *
 * Revision Log :
 * - 0.0.1 (Sep/2016) Module created                        Milos Vidojevic
 *
 * To Do List :
 * @todo
 *
 ******************************************************************************/

#include "nfc_tag2_hal.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

#define READ_BIT                1
#define WRITE_BIT               0

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

#if defined( __MIKROC_PRO_FOR_PIC32__ ) || \
    defined( __MIKROC_PRO_FOR_DSPIC__ )
static uint8_t NACK_BIT   = 0x01;
static uint8_t ACK_BIT    = 0x00;
#elif defined( __MIKROC_PRO_FOR_PIC__ ) || \
      defined( __MIKROC_PRO_FOR_AVR__ )
static uint8_t NACK_BIT   = 0x00;
static uint8_t ACK_BIT    = 0x01;
#endif

#if defined( __MIKROC_PRO_FOR_ARM__ )
    #if defined( __STM32__ ) || defined( __KINETIS__ ) || defined( __MCHP__ )
static unsigned int ( *start_i2c_p )            ( void );
static unsigned int ( *write_i2c_p )            ( unsigned char slave_address,
                                                  unsigned char *buffer,
                                                  unsigned long count,
                                                  unsigned long end_mode );
static void         ( *read_i2c_p )             ( unsigned char slave_address,
                                                  unsigned char *buffer,
                                                  unsigned long count,
                                                  unsigned long end_mode );
    #elif defined( __TI__ )
static void         ( *enable_i2c_p )           ( void );
static void         ( *disable_i2c_p )          ( void );
static void         ( *set_slave_address_i2c_p )( char slave_address,
                                                  char dir );
static unsigned char( *write_i2c_p )            ( unsigned char data_in,
                                                  unsigned char mode );
static unsigned char( *read_i2c_p )             ( unsigned char *data_out,
                                                  unsigned char mode );
    #endif
#elif  defined( __MIKROC_PRO_FOR_AVR__ )
static unsigned char( *busy_i2c_p )             ( void );
static unsigned char( *status_i2c_p )           ( void );
static unsigned char( *start_i2c_p )            ( void );
static void         ( *stop_i2c_p )             ( void );
static void         ( *close_i2c_p )            ( void );
static void         ( *write_i2c_p )            ( unsigned char data_out );
static unsigned char( *read_i2c_p )             ( unsigned char ack );

#elif  defined( __MIKROC_PRO_FOR_PIC__ )
static unsigned char( *is_idle_i2c_p )          ( void );
static unsigned char( *start_i2c_p )            ( void );
static void         ( *stop_i2c_p )             ( void );
static void         ( *restart_i2c_p )          ( void );
static unsigned char( *write_i2c_p )            ( unsigned char data_out );
static unsigned char( *read_i2c_p )             ( unsigned char ack );

#elif defined( __MIKROC_PRO_FOR_PIC32__ )
static unsigned int ( *is_idle_i2c_p )          ( void );
static unsigned int ( *start_i2c_p )            ( void );
static void         ( *stop_i2c_p )             ( void );
static unsigned int ( *restart_i2c_p )          ( void );
static unsigned int ( *write_i2c_p )            ( unsigned char data_out );
static unsigned char( *read_i2c_p )             ( unsigned int ack );

#elif defined( __MIKROC_PRO_FOR_DSPIC__ )
static unsigned int ( *is_idle_i2c_p )          ( void );
static unsigned int ( *start_i2c_p )            ( void );
static void         ( *stop_i2c_p )             ( void );
static void         ( *restart_i2c_p )          ( void );
static unsigned int ( *write_i2c_p )            ( unsigned char data_out );
static unsigned char( *read_i2c_p )             ( unsigned int ack );

#elif defined( __MIKROC_PRO_FOR_8051__ )
static unsigned char( *status_i2c_p )           ( void );
static unsigned char( *start_i2c_p )            ( void );
static void         ( *stop_i2c_p )             ( void );
static void         ( *close_i2c_p )            ( void );
static void         ( *write_i2c_p )            ( unsigned char data_out );
static unsigned char( *read_i2c_p )             ( unsigned char ack );

#elif defined( __MIKROC_PRO_FOR_FT90x__ )
static void         ( *soft_reset_i2c_p )       ( void );
static void         ( *set_slave_address_i2c_p )( unsigned char slave_address );
static unsigned char( *write_i2c_p )            ( unsigned char *buffer,
                                                  unsigned int count );
static unsigned char( *read_i2c_p )             ( unsigned char *buffer,
                                                  unsigned int count );
#endif

/******************************************************************************
* Function Definitions
*******************************************************************************/

void hal_delay( uint16_t ms )
{
#if defined(__MIKROC_PRO_FOR_ARM__)     || \
    defined( __MIKROC_PRO_FOR_FT90x__ ) || \
    defined( __MIKROC_PRO_FOR_AVR__ )   || \
    defined( __MIKROC_PRO_FOR_8051__ )  || \
    defined( __MIKROC_PRO_FOR_DSPIC__ ) || \
    defined( __MIKROC_PRO_FOR_PIC32__ ) || \
    defined( __MIKROC_PRO_FOR_PIC__ )
    while( ms-- )
        Delay_1ms();
#endif
}

int hal_nfctag2_init( void )
{
#if defined( __MIKROC_PRO_FOR_ARM__ )
    #if defined( __STM32__ ) || defined( __KINETIS__ ) || defined( __MCHP__ )
    start_i2c_p                 = I2C_Start_Ptr;
    write_i2c_p                 = I2C_Write_Ptr;
    read_i2c_p                  = I2C_Read_Ptr;

    #elif defined( __TI__ )
    enable_i2c_p                = I2C_Enable_Ptr;
    disable_i2c_p               = I2C_Disable_Ptr;
    set_slave_address_i2c_p     = I2C_Master_Slave_Addr_Set_Ptr;
    write_i2c_p                 = I2C_Write_Ptr;
    read_i2c_p                  = I2C_Read_Ptr;
    #endif

#elif defined( __MIKROC_PRO_FOR_AVR__ )
    #if defined( __LT64__ )
    busy_i2c_p                  = TWI_Busy;
    status_i2c_p                = TWI_Status;
    close_i2c_p                 = TWI_Close;
    start_i2c_p                 = TWI_Start;
    stop_i2c_p                  = TWI_Stop;
    write_i2c_p                 = TWI_Write;
    read_i2c_p                  = TWI_Read;

    #elif defined( __GT64__ )
    busy_i2c_p                  = TWIC_Busy;
    status_i2c_p                = TWIC_Status;
    close_i2c_p                 = TWIC_Close;
    start_i2c_p                 = TWIC_Start;
    stop_i2c_p                  = TWIC_Stop;
    write_i2c_p                 = TWIC_Write;
    read_i2c_p                  = TWIC_Read;
    #endif

#elif defined( __MIKROC_PRO_FOR_PIC__ )
    is_idle_i2c_p               = I2C1_Is_Idle;
    start_i2c_p                 = I2C1_Start;
    stop_i2c_p                  = I2C1_Stop;
    restart_i2c_p               = I2C1_Repeated_Start;
    write_i2c_p                 = I2C1_Wr;
    read_i2c_p                  = I2C1_Rd;

#elif defined( __MIKROC_PRO_FOR_PIC32__ )
    is_idle_i2c_p               = I2C_Is_Idle_Ptr;
    start_i2c_p                 = I2C_Start_Ptr;
    stop_i2c_p                  = I2C_Stop_Ptr;
    restart_i2c_p               = I2C_Restart_Ptr;
    write_i2c_p                 = I2C_Write_Ptr;
    read_i2c_p                  = I2C_Read_Ptr;

#elif defined( __MIKROC_PRO_FOR_DSPIC__ )
    is_idle_i2c_p               = I2C_Is_Idle_Ptr;
    start_i2c_p                 = I2C_Start_Ptr;
    stop_i2c_p                  = I2C_Stop_Ptr;
    restart_i2c_p               = I2C_Restart_Ptr;
    write_i2c_p                 = I2C_Write_Ptr;
    read_i2c_p                  = I2C_Read_Ptr;

#elif defined( __MIKROC_PRO_FOR_8051__ )
    status_i2c_p                = TWI_Status;
    close_i2c_p                 = TWI_Close;
    start_i2c_p                 = TWI_Start;
    stop_i2c_p                  = TWI_Stop;
    write_i2c_p                 = TWI_Write;
    read_i2c_p                  = TWI_Read;

#elif defined( __MIKROC_PRO_FOR_FT90x__ )
    soft_reset_i2c_p            = I2CM_Soft_Reset_Ptr;
    set_slave_address_i2c_p     = I2CM_Set_Slave_Address_Ptr;
    write_i2c_p                 = I2CM_Write_Bytes_Ptr;
    read_i2c_p                  = I2CM_Read_Bytes_Ptr;
#endif
    return 0;
}

int hal_nfctag2_write
(
        uint8_t i2c_address,
        uint8_t* buffer,
        uint16_t count
)
{
    int res = 0;
    uint8_t *ptr = buffer;

#if defined(__MIKROC_PRO_FOR_ARM__)
    #if defined( __STM32__ ) || defined( __KINETIS__ ) || defined( __MCHP__ )
    res |= start_i2c_p();
    res |= write_i2c_p( i2c_address, ptr, count, END_MODE_STOP );

    #elif defined( __TI__ )
    set_slave_address_i2c_p( i2c_address, _I2C_DIR_MASTER_TRANSMIT );

    if( count == 1 ){
        res |= write_i2c_p( *ptr, _I2C_MASTER_MODE_SINGLE_SEND );
    }
    else if( count == 2 ){
        res |= write_i2c_p( *ptr++, _I2C_MASTER_MODE_BURST_SEND_START );
        res |= write_i2c_p( *ptr, _I2C_MASTER_MODE_BURST_SEND_FINISH );
    } else {
        res |= write_i2c_p( *ptr++, _I2C_MASTER_MODE_BURST_SEND_START );
        while( count-- > 1 )
            res |= write_i2c_p( *ptr++, _I2C_MASTER_MODE_BURST_SEND_CONT );
        res |= write_i2c_p( *ptr, _I2C_MASTER_MODE_BURST_SEND_FINISH );
    }
    #endif

#elif defined( __MIKROC_PRO_FOR_FT90x__ )
    set_slave_address_i2c_p( i2c_address );
    res |= write_i2c_p( ptr, count );

#elif defined( __MIKROC_PRO_FOR_AVR__ )   || \
      defined( __MIKROC_PRO_FOR_8051__ )  || \
      defined( __MIKROC_PRO_FOR_DSPIC__ ) || \
      defined( __MIKROC_PRO_FOR_PIC32__ ) || \
      defined( __MIKROC_PRO_FOR_PIC__ )
    res |= start_i2c_p();
    res |= write_i2c_p( ( i2c_address << 1 ) | WRITE_BIT );
    while( count-- )
        res |= write_i2c_p( *ptr++ );
    stop_i2c_p();
#endif
    return res;
}

int hal_nfctag2_read
(
        uint8_t i2c_address,
        uint8_t* buffer,
        uint16_t count 
)
{
    int res = 0;
    uint8_t *ptr = buffer;

#if defined(__MIKROC_PRO_FOR_ARM__)
    #if defined( __STM32__ ) || defined( __KINETIS__ ) || defined( __MCHP__ )
    res |= start_i2c_p();
    res |= write_i2c_p( i2c_address, ptr, 1, END_MODE_STOP );
    res |= start_i2c_p();
    read_i2c_p( i2c_address, ptr, count, END_MODE_STOP );

    #elif defined( __TI__ )
    set_slave_address_i2c_p( i2c_address, _I2C_DIR_MASTER_TRANSMIT );
    res |= write_i2c_p( *ptr, _I2C_MASTER_MODE_SINGLE_SEND );
    set_slave_address_i2c_p( i2c_address, _I2C_DIR_MASTER_RECEIVE );

    if( count == 1 ) {
        res |= read_i2c_p( ptr, _I2C_MASTER_MODE_SINGLE_RECEIVE );
    } 
    else if( count == 2 ){
        res |= read_i2c_p( ptr++, _I2C_MASTER_MODE_BURST_RECEIVE_START );
        res |= read_i2c_p( ptr, _I2C_MASTER_MODE_BURST_RECEIVE_FINISH );
    }
    else {
        res |= read_i2c_p( ptr++ , _I2C_MASTER_MODE_BURST_RECEIVE_START );
        while( --count > 1 )
            res |= read_i2c_p( ptr++ , _I2C_MASTER_MODE_BURST_SEND_CONT );
        res |= read_i2c_p( ptr, _I2C_MASTER_MODE_BURST_SEND_FINISH );
    }
    #endif

#elif defined( __MIKROC_PRO_FOR_FT90x__ )
    set_slave_address_i2c_p( i2c_address );
    res |= write_i2c_p( ptr, 1 );
    res |= read_i2c_p( ptr, count );

#elif defined( __MIKROC_PRO_FOR_AVR__ )    || \
      defined( __MIKROC_PRO_FOR_PIC32__ )  || \
      defined( __MIKROC_PRO_FOR_8051__ )   || \
      defined( __MIKROC_PRO_FOR_PIC__ )    || \
      defined( __MIKROC_PRO_FOR_DSPIC__ )
    res |= start_i2c_p();
    res |= write_i2c_p( ( i2c_address << 1 ) | WRITE_BIT );
    res |= write_i2c_p( *ptr );
    stop_i2c_p();
    Delay_ms( 5 );
    res |= start_i2c_p();
    res |= write_i2c_p( ( i2c_address << 1 ) | READ_BIT );
    while( --count )
        *ptr++ = read_i2c_p( ACK_BIT );
    *ptr = read_i2c_p( NACK_BIT );
    stop_i2c_p();
#endif
    return res;
}

int hal_nfctag2_transfer
(
        uint8_t i2c_address,
        uint8_t* input,
        uint16_t in_count,
        uint8_t* output,
        uint16_t out_count
)
{
    int res = 0;
    uint8_t *in = input;
    uint8_t *out = output;

#if defined(__MIKROC_PRO_FOR_ARM__)
    #if defined( __STM32__ ) || defined( __KINETIS__ ) || defined( __MCHP__ )
    res |= start_i2c_p();
    res |= write_i2c_p( i2c_address, in, in_count, END_MODE_STOP );
    res |= start_i2c_p();
    read_i2c_p( i2c_address, out, out_count, END_MODE_STOP );

    #elif defined( __TI__ )
    set_slave_address_i2c_p( i2c_address, _I2C_DIR_MASTER_TRANSMIT );

    if( in_count == 1 ) {
        res |= write_i2c_p( *in, _I2C_MASTER_MODE_SINGLE_SEND );
    }
    if( in_count == 2 ) {
        res |= write_i2c_p( *in++, _I2C_MASTER_MODE_BURST_SEND_START );
        res |= write_i2c_p( *in, _I2C_MASTER_MODE_BURST_SEND_FINISH );
    }
    else {
        res |= write_i2c_p( *in++, _I2C_MASTER_MODE_BURST_SEND_START );
        while( in_count-- > 1 )
            res |= write_i2c_p( *in++, _I2C_MASTER_MODE_BURST_SEND_CONT );
        res |= write_i2c_p( *in, _I2C_MASTER_MODE_BURST_SEND_FINISH );
    }
    set_slave_address_i2c_p( _i2c_hw_address, _I2C_DIR_MASTER_RECEIVE );

    if( out_count == 1 ) {
        res |= read_i2c_p( out, _I2C_MASTER_MODE_SINGLE_RECEIVE );
    }
    else if( out_count == 2 ){
        res |= read_i2c_p( out++, _I2C_MASTER_MODE_BURST_RECEIVE_START );
        res |= read_i2c_p( out, _I2C_MASTER_MODE_BURST_RECEIVE_FINISH );
    }
    else {
        res |= read_i2c_p( out++ , _I2C_MASTER_MODE_BURST_RECEIVE_START );
        while( --out_count > 1 )
            res |= read_i2c_p( out++ , _I2C_MASTER_MODE_BURST_SEND_CONT );
        res |= read_i2c_p( out, _I2C_MASTER_MODE_BURST_SEND_FINISH );
    }
    #endif

#elif defined( __MIKROC_PRO_FOR_FT90x__ )
    set_slave_address_i2c_p( i2c_address );
    res |= write_i2c_p( in, in_count );
    set_slave_address_i2c_p( i2c_address );
    res |= read_i2c_p( out, out_count );


#elif defined( __MIKROC_PRO_FOR_AVR__ )   || \
      defined( __MIKROC_PRO_FOR_8051__ )  || \
      defined( __MIKROC_PRO_FOR_DSPIC__ ) || \
      defined( __MIKROC_PRO_FOR_PIC32__ ) || \
      defined( __MIKROC_PRO_FOR_PIC__ )
    res |= start_i2c_p();
    res |= write_i2c_p( ( i2c_address << 1 ) | WRITE_BIT );
    while( in_count-- )
        res|= write_i2c_p( *in++ );
    stop_i2c_p();
    res |= start_i2c_p();
    res |= write_i2c_p( ( i2c_address << 1 ) | READ_BIT );
    while( --out_count )
        *out++ = read_i2c_p( ACK_BIT );
    *out = read_i2c_p( NACK_BIT );
    stop_i2c_p();
#endif
    return res;
}

/*************** END OF FUNCTIONS *********************************************/