// UART-Funktionen

//UART Berechnung
#define BAUD 9600UL      // Baudrate

#define TERMINATOR 10    // End of String Terminator
 
// Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
 
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  //#error Systematischer Fehler der Baudrate groesser 1% und damit zu hoch! 
#endif


//UART initialisieren
void uart_init(void)
{
	UBRRH = UBRR_VAL >> 8;
	UBRRL = UBRR_VAL & 0xFF;
	
	UCSRB |= (1<<TXEN) | (1<<RXEN) | (1<<RXCIE);  // UART TX, RX
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);  // Asynchron 8N1 

}

//Char schreiben
int uart_putc(unsigned char c)
{
    while (!(UCSRA & (1<<UDRE)))  /* warten bis Senden moeglich */
    {
    }                             
 
    UDR = c;                      /* sende Zeichen */
    return 0;
}

//String schreiben
void uart_puts(char *s)
{
    while (*s)
    {   /* so lange *s != '\0' also ungleich dem "String-Endezeichen(Terminator)" */
        uart_putc(*s);
        s++;
    }
}

void uart_puti(uint16_t i)
{
    char buffer[8];
    itoa(i,buffer,10);
    uart_puts(buffer);
}

//Char lesen
uint8_t uart_getc(void)
{
    while (!(UCSRA & (1<<RXC)))   // warten bis Zeichen verfuegbar
    {
    }
    return UDR;                   // Zeichen aus UDR an Aufrufer zurueckgeben
}

uint8_t uart_getc_tout(uint8_t TOut)
{
  uint16_t tout = 0;
  while (!(UCSRA & (1<<RXC)))   // warten bis Zeichen verfuegbar
  {
    tout++;
    _delay_ms(1);
    if ( tout > TOut )
    {
      return 0;
    }
  }
  return UDR;                   // Zeichen aus UDR an Aufrufer zurueckgeben
}

//String lesen
void uart_gets( char* Buffer, uint8_t MaxLen ) 
{
	uint8_t NextChar;
	uint8_t StringLen = 0;

	NextChar = uart_getc();         // Warte auf und empfange das nächste Zeichen

	                              // Sammle solange Zeichen, bis:
	                              // * entweder das String Ende Zeichen kam
	                              // * oder das aufnehmende Array voll ist
	//while( NextChar != TERMINATOR && StringLen < MaxLen - 1 ) 
	while( NextChar != TERMINATOR )
	{
		
        *Buffer++ = NextChar;
		StringLen++;
		NextChar = uart_getc();
	}
 
                                  // Noch ein '\0' anhängen um einen Standard
                                  // C-String daraus zu machen
  *Buffer = '\0';
}

