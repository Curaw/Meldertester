/************************************************************************/
/*																		*/
/*			Access Dallas 1-Wire Devices								*/
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/
#ifndef _1wire_h_
#define _1wire_h_

#ifndef W1_PIN
#define W1_PIN	PB7
#define W1_IN	PINB
#define W1_OUT	PORTB
#define W1_DDR	DDRB
#endif

#define MATCH_ROM	0x55
#define SKIP_ROM	0xCC
#define	SEARCH_ROM	0xF0

#define CONVERT_T	0x44		// DS1820 commands
#define READ		0xBE
#define WRITE		0x4E
#define EE_WRITE	0x48
#define EE_RECALL	0xB8

#define	SEARCH_FIRST	0xFF		// start new search
#define	PRESENCE_ERR	0xFF
#define	DATA_ERR	0xFE
#define LAST_DEVICE	0x00		// last device found
//			0x01 ... 0x40: continue searching


bit w1_reset(void)
{
  bit err;

  W1_OUT &= ~(1<<W1_PIN);
  W1_DDR |= 1<<W1_PIN;
  _delay_us(480);			// 480 us
  cli();
  W1_DDR &= ~(1<<W1_PIN);
  _delay_us(66);
  err = W1_IN & (1<<W1_PIN);			// no presence detect
  sei();
  _delay_us(480 - 66);
  if( (W1_IN & (1<<W1_PIN)) == 0 )		// short circuit
    err = 1;
  return err;
}


uchar w1_bit_io( bit b )
{
  cli();
  W1_DDR |= 1<<W1_PIN;
  _delay_us(1);
  if( b )
    W1_DDR &= ~(1<<W1_PIN);
  _delay_us(15 - 1);
  if( (W1_IN & (1<<W1_PIN)) == 0 )
    b = 0;
  _delay_us(60 - 15);
  W1_DDR &= ~(1<<W1_PIN);
  sei();
  return b;
}


uint w1_byte_wr( uchar b )
{
  uchar i = 8, j;
  do{
    j = w1_bit_io( b & 1 );
    b >>= 1;
    if( j )
      b |= 0x80;
  }while( --i );
  return b;
}


uint w1_byte_rd( void )
{
  return w1_byte_wr( 0xFF );
}

uchar w1_rom_search( uchar diff, uchar idata *id )
{
	uchar i, j, next_diff;
	bit b;

	if( w1_reset() )
	return PRESENCE_ERR;			// error, no device found
	w1_byte_wr( SEARCH_ROM );			// ROM search command
	next_diff = LAST_DEVICE;			// unchanged on last device
	i = 8 * 8;					// 8 bytes
	do{
		j = 8;					// 8 bits
		do{
			b = w1_bit_io( 1 );			// read bit
			if( w1_bit_io( 1 ) ){			// read complement bit
				if( b )					// 11
				return DATA_ERR;			// data error
				}else{
				if( !b ){				// 00 = 2 devices
					if( diff > i ||
					((*id & 1) && diff != i) ){
						b = 1;				// now 1
						next_diff = i;			// next pass 0
					}
				}
			}
			w1_bit_io( b );     			// write bit
			*id >>= 1;
			if( b )					// store bit
			*id |= 0x80;
			i--;
		}while( --j );
		id++;					// next byte
	}while( i );
	return next_diff;				// to continue search
}

void w1_command( uchar command, uchar idata *id )
{
  uchar i;
  w1_reset();
  if( id ){
    w1_byte_wr( MATCH_ROM );			// to a single device
    i = 8;
    do{
      w1_byte_wr( *id );
      id++;
    }while( --i );
  }else{
    w1_byte_wr( SKIP_ROM );			// to all devices
  }
  w1_byte_wr( command );
}
#endif