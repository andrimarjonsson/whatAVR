#include "clcd.h"

/*
 * DEFINITONS
 */

#define lcd_e_delay()   __asm__ __volatile__( "rjmp 1f\n 1:" )
#define lcd_e_high()    LCD_E_PORT  |=  ( 1<<LCD_E_PIN )
#define lcd_e_low()     LCD_E_PORT  &= ~( 1<<LCD_E_PIN )
#define lcd_e_toggle()  toggle_e( )
#define lcd_rw_high()   LCD_RW_PORT |=  ( 1<<LCD_RW_PIN )
#define lcd_rw_low()    LCD_RW_PORT &= ~( 1<<LCD_RW_PIN )
#define lcd_rs_high()   LCD_RS_PORT |=  ( 1<<LCD_RS_PIN )
#define lcd_rs_low()    LCD_RS_PORT &= ~( 1<<LCD_RS_PIN )

#if LCD_LINES==1
	#define LCD_FUNCTION_DEFAULT    LCD_FUNCTION_4BIT_1LINE 
#else
	#define LCD_FUNCTION_DEFAULT    LCD_FUNCTION_4BIT_2LINES 
#endif


/*
 * PRIVATE
 */

static void toggle_e( void )
{
    lcd_e_high();
    lcd_e_delay();
    lcd_e_low();
}

static void lcd_write( uint8_t data, uint8_t rs ) 
{
    if ( rs ) 
	{   /* write data        (RS=1, RW=0) */
       lcd_rs_high();
    }
	else
	{    /* write instruction (RS=0, RW=0) */
       lcd_rs_low();
    }

    lcd_rw_low();

	/* configure data pins as output */
	DDR(LCD_DATA0_PORT) |= (1<<LCD_DATA0_PIN);
	DDR(LCD_DATA1_PORT) |= (1<<LCD_DATA1_PIN);
	DDR(LCD_DATA2_PORT) |= (1<<LCD_DATA2_PIN);
	DDR(LCD_DATA3_PORT) |= (1<<LCD_DATA3_PIN);

	/* output high nibble first */
	LCD_DATA3_PORT &= ~(1<<LCD_DATA3_PIN);
	LCD_DATA2_PORT &= ~(1<<LCD_DATA2_PIN);
	LCD_DATA1_PORT &= ~(1<<LCD_DATA1_PIN);
	LCD_DATA0_PORT &= ~(1<<LCD_DATA0_PIN);
	
	if(data & 0x80) 
		LCD_DATA3_PORT |= (1<<LCD_DATA3_PIN);
	if(data & 0x40) 
		LCD_DATA2_PORT |= (1<<LCD_DATA2_PIN);
	if(data & 0x20) 
		LCD_DATA1_PORT |= (1<<LCD_DATA1_PIN);
	if(data & 0x10) 
		LCD_DATA0_PORT |= (1<<LCD_DATA0_PIN);   
	lcd_e_toggle();

	/* output low nibble */
	LCD_DATA3_PORT &= ~(1<<LCD_DATA3_PIN);
	LCD_DATA2_PORT &= ~(1<<LCD_DATA2_PIN);
	LCD_DATA1_PORT &= ~(1<<LCD_DATA1_PIN);
	LCD_DATA0_PORT &= ~(1<<LCD_DATA0_PIN);
	
	if(data & 0x08) 
		LCD_DATA3_PORT |= (1<<LCD_DATA3_PIN);
	if(data & 0x04) 
		LCD_DATA2_PORT |= (1<<LCD_DATA2_PIN);
	if(data & 0x02) 
		LCD_DATA1_PORT |= (1<<LCD_DATA1_PIN);
	if(data & 0x01) 
		LCD_DATA0_PORT |= (1<<LCD_DATA0_PIN);
	lcd_e_toggle();        

	/* all data pins high (inactive) */
	LCD_DATA0_PORT |= (1<<LCD_DATA0_PIN);
	LCD_DATA1_PORT |= (1<<LCD_DATA1_PIN);
	LCD_DATA2_PORT |= (1<<LCD_DATA2_PIN);
	LCD_DATA3_PORT |= (1<<LCD_DATA3_PIN);
	
}

static uint8_t lcd_read( uint8_t rs ) 
{
    uint8_t data;
    
    if ( rs )
        lcd_rs_high();                       /* RS=1: read data      */
    else
        lcd_rs_low();                        /* RS=0: read busy flag */
    
	lcd_rw_high();                           /* RW=1  read mode      */    

	/* configure data pins as input */
	DDR(LCD_DATA0_PORT) &= ~(1<<LCD_DATA0_PIN);
	DDR(LCD_DATA1_PORT) &= ~(1<<LCD_DATA1_PIN);
	DDR(LCD_DATA2_PORT) &= ~(1<<LCD_DATA2_PIN);
	DDR(LCD_DATA3_PORT) &= ~(1<<LCD_DATA3_PIN);
	
	/* enable pullups*/
    LCD_DATA0_PORT &= ~(1<<LCD_DATA0_PIN);
	LCD_DATA1_PORT &= ~(1<<LCD_DATA1_PIN);
	LCD_DATA2_PORT &= ~(1<<LCD_DATA2_PIN);
	LCD_DATA3_PORT &= ~(1<<LCD_DATA3_PIN);
    
	/* read high nibble first */
	lcd_e_high();
	lcd_e_delay();        
	data = 0;
	if ( PIN(LCD_DATA0_PORT) & (1<<LCD_DATA0_PIN) ) 
		data |= 0x10;
	if ( PIN(LCD_DATA1_PORT) & (1<<LCD_DATA1_PIN) ) 
		data |= 0x20;
	if ( PIN(LCD_DATA2_PORT) & (1<<LCD_DATA2_PIN) ) 
		data |= 0x40;
	if ( PIN(LCD_DATA3_PORT) & (1<<LCD_DATA3_PIN) ) 
		data |= 0x80;
	lcd_e_low();

	lcd_e_delay();                       /* Enable 500ns low       */

	/* read low nibble */    
	lcd_e_high();
	lcd_e_delay();
	if ( PIN(LCD_DATA0_PORT) & (1<<LCD_DATA0_PIN) ) 
		data |= 0x01;
	if ( PIN(LCD_DATA1_PORT) & (1<<LCD_DATA1_PIN) ) 
		data |= 0x02;
	if ( PIN(LCD_DATA2_PORT) & (1<<LCD_DATA2_PIN) ) 
		data |= 0x04;
	if ( PIN(LCD_DATA3_PORT) & (1<<LCD_DATA3_PIN) ) 
		data |= 0x08;        
	lcd_e_low();

    return data;
}

static uint8_t lcd_waitbusy( void )
{
    register uint8_t c;
    
    /* wait until busy flag is cleared */
    while ( ( c = lcd_read( 0 ) ) & ( 1<<LCD_BUSY )) {}
    
    /* the address counter is updated 4us after the busy flag is cleared */
    _delay_us(5);

    /* now read the address counter */
    return ( lcd_read( 0 ) );  // return address counter
    
}

static inline void lcd_newline( uint8_t pos )
{
    register uint8_t addressCounter;

#if LCD_LINES==1
    addressCounter = 0;
#endif

#if LCD_LINES==2
    if ( pos < LCD_START_LINE2 )
        addressCounter = LCD_START_LINE2;
    else
        addressCounter = LCD_START_LINE1;
#endif

#if LCD_LINES==4
    if ( pos < LCD_START_LINE3 )
        addressCounter = LCD_START_LINE2;
    else if ( ( pos >= LCD_START_LINE2 ) && ( pos < LCD_START_LINE4 ) )
        addressCounter = LCD_START_LINE3;
    else if ( ( pos >= LCD_START_LINE3 ) && ( pos < LCD_START_LINE2 ) )
        addressCounter = LCD_START_LINE4;
    else 
        addressCounter = LCD_START_LINE1;
#endif
    lcd_command( ( 1<<LCD_DDRAM ) + addressCounter);
}


/*
 * PUBLIC
 */
void lcd_init( uint8_t dispAttr )
{
    /*
     *  Initialize LCD to 4 bit I/O mode
     */
	DDR( LCD_RS_PORT )    |= ( 1<<LCD_RS_PIN );
	DDR( LCD_RW_PORT )    |= ( 1<<LCD_RW_PIN );
	DDR( LCD_E_PORT )     |= ( 1<<LCD_E_PIN );
	DDR( LCD_DATA0_PORT ) |= ( 1<<LCD_DATA0_PIN );
	DDR( LCD_DATA1_PORT ) |= ( 1<<LCD_DATA1_PIN );
	DDR( LCD_DATA2_PORT ) |= ( 1<<LCD_DATA2_PIN );
	DDR( LCD_DATA3_PORT ) |= ( 1<<LCD_DATA3_PIN );
    
    _delay_ms( 16 );
    
    /* initial write to lcd is 8bit */
    LCD_DATA1_PORT |= ( 1<<LCD_DATA1_PIN );  // _BV(LCD_FUNCTION)>>4;
    LCD_DATA0_PORT |= ( 1<<LCD_DATA0_PIN );  // _BV(LCD_FUNCTION_8BIT)>>4;
    lcd_e_toggle( );
    _delay_ms( 5 );         /* delay, busy flag can't be checked here */
   
    /* repeat last command */ 
    lcd_e_toggle( );      
    _delay_us( 64 );           /* delay, busy flag can't be checked here */
    
    /* repeat last command a third time */
    lcd_e_toggle( );      
    _delay_us( 64 );           /* delay, busy flag can't be checked here */

    /* now configure for 4bit mode */
    LCD_DATA0_PORT &= ~( 1<<LCD_DATA0_PIN );   // LCD_FUNCTION_4BIT_1LINE>>4
    lcd_e_toggle( );
    _delay_us( 64 );           /* some displays need this additional delay */
    
    /* from now the LCD only accepts 4 bit I/O, we can use lcd_command() */    

    lcd_command( LCD_FUNCTION_DEFAULT );      /* function set: display lines  */

    lcd_command( LCD_DISP_OFF );            /* display off                  */
    lcd_clrscr( );							/* display clear                */ 
    lcd_command( LCD_MODE_DEFAULT );        /* set entry mode               */
    lcd_command( dispAttr );                /* display/cursor control       */
}

void lcd_clrscr( void )
{
	lcd_command( 1<<LCD_CLR );
}

void lcd_home( void )
{
	lcd_command( 1<<LCD_HOME );
}

void lcd_gotoxy( uint8_t x, uint8_t y )
{
#if LCD_LINES==1
    lcd_command( ( 1<<LCD_DDRAM ) + LCD_START_LINE1 + x );
#endif

#if LCD_LINES==2
    if ( y == 0 ) 
        lcd_command( ( 1<<LCD_DDRAM ) + LCD_START_LINE1 + x );
    else /* y==2 */
        lcd_command( ( 1<<LCD_DDRAM ) + LCD_START_LINE2 + x );
#endif

#if LCD_LINES==4
    if ( y == 0 )
        lcd_command( ( 1<<LCD_DDRAM ) + LCD_START_LINE1 + x );
    else if ( y == 1 )
        lcd_command( ( 1<<LCD_DDRAM ) + LCD_START_LINE2 + x );
    else if ( y == 2 )
        lcd_command( ( 1<<LCD_DDRAM ) + LCD_START_LINE3 + x );
    else /* y==3 */
        lcd_command( ( 1<<LCD_DDRAM ) + LCD_START_LINE4 + x );
#endif
}

void lcd_putc( char c )
{
    uint8_t pos;

    pos = lcd_waitbusy( );   // read busy-flag and address counter
    if ( c=='\n' )
		lcd_newline( pos );
    else
    {
#if LCD_WRAP_LINES==1
#if LCD_LINES==1
        if ( pos == LCD_START_LINE1+LCD_DISP_LENGTH )
            lcd_write( ( 1<<LCD_DDRAM ) + LCD_START_LINE1, 0 );
#elif LCD_LINES==2
		if ( pos == LCD_START_LINE1+LCD_DISP_LENGTH ) 
			lcd_write( ( 1<<LCD_DDRAM ) + LCD_START_LINE2, 0 );
		else if ( pos == LCD_START_LINE2+LCD_DISP_LENGTH )
			lcd_write( ( 1<<LCD_DDRAM ) + LCD_START_LINE1, 0 );
#elif LCD_LINES==4
		if ( pos == LCD_START_LINE1+LCD_DISP_LENGTH ) 
			lcd_write( ( 1<<LCD_DDRAM ) + LCD_START_LINE2, 0 );    
		else if ( pos == LCD_START_LINE2+LCD_DISP_LENGTH ) 
			lcd_write( ( 1<<LCD_DDRAM ) + LCD_START_LINE3, 0 );
		else if ( pos == LCD_START_LINE3+LCD_DISP_LENGTH ) 
			lcd_write( ( 1<<LCD_DDRAM ) + LCD_START_LINE4, 0 );
		else if ( pos == LCD_START_LINE4+LCD_DISP_LENGTH ) 
			lcd_write( ( 1<<LCD_DDRAM ) + LCD_START_LINE1, 0 );
#endif
		lcd_waitbusy( );
#endif
        lcd_write( c, 1 );
	}
}

void lcd_puts( const char *s )
{
	register char c;
    while ( (c = *s++) ) 
        lcd_putc( c );
}

void lcd_command( uint8_t cmd )
{
	lcd_waitbusy( );
    lcd_write( cmd, 0 );	
}

void lcd_data( uint8_t data )
{
	lcd_waitbusy( );
    lcd_write( data, 1 );
}

void lcd_put_string_float( const char *s, uint8_t decimalLoc )
{
    register char c;
    uint8_t i = 0;
    while ( ( c = *s++ ) )
    {
        if ( i == decimalLoc )
            lcd_putc( '.' );
        lcd_putc( c );
        i++;
    }
}

#if LCD_HAVE_PROGMEM_SUPPORT
void lcd_puts_p( const char *progmem_s )
{
	register char c;

    while ( ( c = pgm_read_byte( progmem_s++ ) ) ) 
        lcd_putc( c );
}
#endif
