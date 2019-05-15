
/*
 * Grecon.c
 *
 * Created: 19.04.2019 16:54:36
 * Author : Daniel Kison
 */ 

//Wichtige TODOS:
//1. Der Flash-Speicher vom Mikrocontroller ist voll. Alle Booleans gemeinsam in ein oder mehrere Flag-Bytes speichern
//2. Die Buchstaben sollten dringend per Char Array im Code gezeichnet werden koennen und nicht ueber Zahlen
//3. Es sind nicht alle Buchstaben einkommentiert, wegen Punkt 1
//4. Alle anderen TODOS stehen im Code

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#define uchar unsigned char
#define uint unsigned int
#define bit uchar
#define idata

#include "displayDriver.c"
#include "1WIRE.c"
#include "uart.h"

//Trigger fuer den Barcodescanner
volatile uint8_t uart_str_complete = 0;     // 1 .. String komplett empfangen
volatile uint8_t uart_str_count = 0;
#define UART_TERMINATOR 10 
#define UART_MAXSTRLEN 20
volatile char uart_string[UART_MAXSTRLEN + 1] = "";
uint8_t uart_str_len;
#define Trigger PD4
#define triggerOn  PORTD |= (1 << Trigger);
#define Trigeroff PORTD &= ~(1 << Trigger);
//#define LEDtog PORTD ^= (1 << LED);

//Display befindet sich in displayDriver.c
uint8_t selectedLine;
uint8_t selectedType;
uint8_t updateType;
uint8_t selectedPoti;
uint8_t updatePoti;
uint8_t arrowCounter;
uint8_t showArrow;
uint8_t isDLD;		//TODO: Alle Booleans in einer einzelnen Variable "flags" verstauen, um Platz zu sparen
uint8_t hourglassCounter;
uint8_t hourglassAnimCounter;
uint8_t showHourglass;
uint8_t updateHourglassAnim;
uint8_t hourglassOnScan;

//Timer fuer die Sanduhr
#define _TIM0_INT_en    TIMSK |= (1 << TOIE1);
#define _TIM0_INT_dis   TIMSK &= ~(1 << TOIE1);
#define _RXCIE_INT_en    UCSRB |= (1 << RXCIE);
#define _RXCIE_INT_dis   UCSRB &= ~(1 << RXCIE);

//Encoder
#define BUTTON_MASK_0 (1<<PC0)
#define BUTTON_MASK_1 (1<<PC1)
#define BUTTON_MASK_2 (1<<PC2)
#define BUTTON_PIN PINC
#define BUTTON_PORT PORTC
volatile uint8_t button_down;	//1 = Encoder wurde gedrueckt

//Messung
//EEPROM
#define EEPROM_DEF 0xFF

//DAC-Wandler
#define reg_portDAC PORTB
#define reg_dataDAC PB0
#define reg_sclkDAC PB1
#define reg_csDAC   PB2

//globale initialisierungen
uint16_t eeFooWordArray1[5] EEMEM; //eeprom

// Diese Funktion sorgt dafuer, dass jeder Buttondruck auch nur einmal erkannt wird
static inline void debounce(void)
{
	// "Frame" counter
	static uint8_t frameCount = 0;
	// Der Zustand, der von der Logik angenommen wird
	static uint8_t button_state = 0;
	// Testet, ob der Button gerade in diesem Frame gedrueckt ist
	uint8_t current_state = (~BUTTON_PIN & BUTTON_MASK_1) != 0;
	if (current_state != button_state) {
		// In diesem Frame wurde der Button gedrueckt also Framecounter erhoehen
		frameCount++;
		if (frameCount >= 4) {
			// Nach 4 Frames akzeptieren wir den neuen ButtonInput
			button_state = current_state;
			// Der main-Funktion sagen, dass der Button jetzt offiziell gedrueckt ist
			if (current_state != 0) {
				button_down = 1;
			}
			frameCount = 0;
		}
		} else {
		// Nicht lange genug gedrueckt
		frameCount = 0;
	}
}

//0 = keine Drehung
//1 = im Uhrzeigersinn
//2 = gegen Uhrzeigersinn
uint8_t read_encoder_direction(void )
{
	uint8_t direction = 0;
	uint8_t buttonState_0, buttonState_2;
	
	//Die Richtung der Drehung wird bei diesem Encoder erkannt, indem ueberprueft wird,
	//welcher der beiden Pins (0 oder 2) zuerst gesetzt ist.
	//Wenn der Encoderschalter einrastet, dann stehen beide Pins wieder auf 0.
	if((~BUTTON_PIN & BUTTON_MASK_0)) {
		//Pin 0 wurde erkannt...
		buttonState_0 = 1;
		buttonState_2 = (~BUTTON_PIN & BUTTON_MASK_2) != 0;
		//... wenn jetzt Pin 2 noch nicht gesetzt ist, dann war Pin 0 der erste gesetzte
		if(buttonState_2 == 1) {
			direction = 2;
		} else {
			direction = 1;
		}
	} else if((~BUTTON_PIN & BUTTON_MASK_2)) {
		//Pin 2 wurde erkannt...
		buttonState_2 = 1;
		buttonState_0 = (~BUTTON_PIN & BUTTON_MASK_0) != 0;
		//... wenn jetzt Pin 0 noch nicht gesetzt ist, dann war Pin 2 der erste gesetzte
		if(buttonState_0 == 1) {
			direction = 1;
			} else {
			direction = 2;
		}
	}
	return direction;
}

//Verursacht einen Wechsel im Menu von DLD auf FM und umgekehrt
void changeType() {
	if(selectedType == 0) {
		selectedType = 1;
		} else {
		selectedType = 0;
	}
	updateType = 1;
}

//Verursacht eine Aenderung am POTI-Wert im Menu
void changePoti(uint8_t direction) {
	if(direction == 1) {
		if(selectedPoti == 10) {
			selectedPoti = 0;
		} else {
			selectedPoti++;
		}
		} else {
		if(selectedPoti == 0) {
			selectedPoti = 10;
		} else {
			selectedPoti--;
		}
	}
	updatePoti = 1;
}

//Reaktion auf die Drehung des Encoders
void buttonRotation(uint8_t direction) {
	switch(selectedLine) {
		case 0:
			//Do nothing
			break;
		case 1:
			changeType();
			break;
		case 2:
			changePoti(direction);
			break;
		case 3:
			//Do nothing
			break;
		default:
			break;
	}
}

//Zeigt die gemessene Umgebungstemperatur zweistellig an
void displayTemp(int dispTemp) {
	int newTemp = dispTemp * 0.0625;
	uint8_t einer = newTemp % 10;
	uint8_t zehner = (newTemp / 10) % 10;
	OLED_ReadMemLetter(zehner + 56);
	OLED_DispImage(letterBuffer,39,5,8,10);
	OLED_ReadMemLetter(einer + 56);
	OLED_DispImage(letterBuffer,42,5,8,10);
}

//Temperaturmessung
void start_meas( void ){
	if( W1_IN & 1<< W1_PIN ){
		w1_command( CONVERT_T, NULL );
		W1_OUT |= 1<< W1_PIN;
		W1_DDR |= 1<< W1_PIN;
	}
}

//Temperaturlesung
void read_meas( void ) {
	uchar id[8], diff;
	uint temp = 0;

	for( diff = SEARCH_FIRST; diff != LAST_DEVICE; ){
		diff = w1_rom_search( diff, id );

		if( diff == PRESENCE_ERR ){
			//No Sensor found
			OLED_ClearLetter(42,5);
			OLED_ClearLetter(39,5);
			return;
		}
		if( diff == DATA_ERR ){
			//Bus Error
			return;
		}
		if( id[0] == 0x28 || id[0] == 0x10 ){	// Temperatursensor
			w1_byte_wr( READ );			// Read command
			temp = w1_byte_rd();			// Low Byte
			temp |= (uint)w1_byte_rd() << 8;		// High Byte
			if( id[0] == 0x10 )			// 9 -> 12 Bit
			temp <<= 3;
		}
	}
	displayTemp(temp);
}

//Fuehrt eine Messung der Umgebungstemperatur komplett durch
void measureTemp() {
	start_meas();
	_delay_ms(1000);
	read_meas();
}

//DAC-Wandler funktion von Ede. 0-1023 digit werden in stromsträrke umgewandelt um die funken für
//die sensoren zu simulieren
void convertDAC(uint16_t data)
{
	reg_portDAC &= ~(1<<reg_sclkDAC);
	reg_portDAC &= ~(1<<reg_csDAC);
	for (int i=0; i<16;i++)
	{
		reg_portDAC &= ~(1<<reg_dataDAC);
		_delay_us(10);
		if(data & 0x8000)
		{
			reg_portDAC |= (1<<reg_dataDAC);
		}
		data <<= 1;
		_delay_us(10);
		reg_portDAC |= (1<<reg_sclkDAC);
		_delay_us(10);
		reg_portDAC &= ~(1<<reg_sclkDAC);
		_delay_us(10);
	}
	_delay_us(10);
	reg_portDAC |= (1<<reg_csDAC);
}

//Zeigt einen neuen Messwert auf dem Display an
void updateMesswert(char newVal[]) {
	
	//Konvertierung des Wertes in einen integer
	int firstNumber = newVal[0] - '0';
	int secondNumber = newVal[1] - '0';
	int thirdNumber = newVal[2] - '0';
	
	OLED_ReadMemLetter(firstNumber + 56);
	OLED_DispImage(letterBuffer,46,46,8,10);
	OLED_ReadMemLetter(secondNumber + 56);
	OLED_DispImage(letterBuffer,49,46,8,10);
	OLED_ReadMemLetter(thirdNumber + 56);
	OLED_DispImage(letterBuffer,52,46,8,10);
}

//Funktion fuer die Messung (Aus dem alten Projekt, von EDE).
//schwelle_erkannt: anzahl der messdurchläufe
//sensor: art des sensor, also fm, hpd oder dld, benötigt für abfrage für schwelle
//kalib: kalibriermodus ja = 1 oder nein = 0
//kalib_summe: wo kalibrierter wert im eeprom abgespeichert werden soll
//data: zur auswahl der leuchtquelle und einstellung der lichstärke. standardmäßig ist die stärke 0
void messung (int schwelle_erkannt, uint8_t sensor, uint8_t kalib, int kalib_summe, int16_t data)
{
	int16_t data1 = data;		//benötigt um impulse mittels der leuchtquellen zu erzeugen
	int nerk_abbruch = 0;		//zur prüfung ob sensor sich im gehäuse befindet oder offen rumliegt
	int bin_suche = 1024;		//variable für den binären suchbaum. sie wird addiert oder subtrahiert, je nachdem ob der sensor auslöst oder nicht
	int16_t schwelle1 = 0;		//benötigt für abfrage der schwelle der sensoren
	int16_t schwelle2 = 0;		//benötigt für abfrage der schwelle der sensoren
	int val = 1023;				//wert für dac-wandler
	int erkannt = 0;			//prüfung ob funke erkannt bzw. impuls von sensor abgegeben wurde
	uint32_t all_val = 0;		//alle erkannten schwellen aufsummiert
	int anzahl_erk = 0;			//anzahl der durchläufe durch den binären suchbaum
	uint32_t mittlere_schwelle =0;	//wert aller erkannten schwellen durch die anzahl der durchläufe
	
	OLED_ClearLetter(0,100);
	OLED_ClearLetter(5,100);
	
	//messe so lange bis vorgegebene anzahl an suchdruchläufen erreicht ist
	while(anzahl_erk < schwelle_erkannt)
	{
		//dac wandler: erzeugung eines lichtimpulses mit dac wert welcher durch val vorgegen ist
		data |= ( val << 2);
		convertDAC(data);
		_delay_us(2000);
		//adc wandlung um zu prüfen ob der sensor ausgelöst hat. dazu werden 100 werte über eine bestimmte zeit
		//auf einen adc wert über 900 geprüft. wird ein solcher adc wert erkannt, hat der sensor ausgelöst.
		unsigned int adc_value;
		ADCSRA |= (1<<ADSC);
		while (ADCSRA & (1<<ADSC)){};
		adc_value = ADCW;
		for (int j =0;j<101;j++)
		{
			ADCSRA |= (1<<ADSC);
			while (ADCSRA & (1<<ADSC)){};
			adc_value = ADCW;
			if (adc_value > 900)
			{
				erkannt = 1;
			}
			_delay_us(1500);
		}
		//rücksetzung der stromstärke der lichtquelle auf 0 um impuls zu erzeugen
		data = data1;
		convertDAC(data);
		_delay_ms(1000);
		
		bin_suche = bin_suche/2;
		
		//wenn bei vollauschlag nichts erkannt wird bricht die messung ab
		if (erkannt == 0 && val == 1023)
		{
			//DEBUG-Buchstabe: D
			OLED_ReadMemLetter(4);
			OLED_DispImage(letterBuffer,0,100,8,10);
			//uart_puts("nichts erkannt");
			//uart_init();
			//char buffer[8];
			//uart_gets(buffer,8);
			//uart_puts("\r\n");
			break;
		}
		//wenn sensor auslöst
		if (erkannt == 1)
		{
			//char wert[15];
			val = val - bin_suche;
			//wenn bin_suche < 1 ist ein suchdurchlauf für die schwelle abgeschlossen
			if (bin_suche < 1)
			{
				all_val = all_val + val;
				anzahl_erk++;
				bin_suche = 1024;
				val = 1023;
				nerk_abbruch = 0;
			}
			erkannt = 0;
		}
		//wenn er nicht auslöst
		else
		{
			val= val + bin_suche;
			//falls bin_suche < 1 und die schwelle nicht erkannt wurde wird in 1 digit schritten nach oben
			//gezählt bis sensor auslöst. wenn der sensor nach 5 schritten nicht auslöst wird davon ausgegangen,
			//dass der sensor sich nicht im gehäuse befindet und die messung wird abgebrochen.
			if (bin_suche < 1)
			{
				val = val + 1;
				nerk_abbruch++;
				if (nerk_abbruch >4)
				{
					//DEBUG-Buchstabe: F
					OLED_ReadMemLetter(6);
					OLED_DispImage(letterBuffer,5,100,8,10);
					//uart_puts("Messung fehlerhaft");
					//uart_puts("\r\n");
					break;
				}
			}
		}
	}
	//berechnung der mittleren schwelle
	if (anzahl_erk == schwelle_erkannt)
	{
		mittlere_schwelle = all_val/anzahl_erk;
	}
	
	_delay_ms(100);
	//wenn nicht kalabriert wird, wird hier geprüft, ob sich der sensor innerhalb der schwelle befindet
	//TODO: Die Kalibrierung kann vermutlich raus, da wir alle Einstellungen im Menu treffen werden
	if (kalib == 0)
	{
		schwelle1 = eeprom_read_word (&eeFooWordArray1[sensor]);
		schwelle2 = eeprom_read_word (&eeFooWordArray1[sensor + 1]);
		eeprom_write_word(&eeFooWordArray1[6], mittlere_schwelle);
		if (mittlere_schwelle <= schwelle2 && mittlere_schwelle >= schwelle1)
		{
			PORTD |= (1 << PD7);
		}
		else
		{
			PORTD |= (1 << PD6);
		}
	}
	//ausgabe der gemessenen werte
	if (mittlere_schwelle |= 0)
	{
		char wert[15];
		_delay_ms(5);
		//uart_puts("mitt. schwelle:");
		_delay_ms(5);
		itoa(mittlere_schwelle,wert,10);
		updateMesswert(wert);
		//uart_puts(wert);
		//uart_puts("\r\n");
	}
	//falls kalibirert wird werden hier die kalibrierten werte in den eeprom geschrieben
	// und ausgeben um das ergbnis zu überprüfen
	if (kalib == 1)
	{
		{
			eeprom_write_word(&eeFooWordArray1[kalib_summe], mittlere_schwelle);
			mittlere_schwelle = eeprom_read_word (&eeFooWordArray1[kalib_summe]);
			char wert[15];
			//uart_puts("eeprom:");
			itoa(mittlere_schwelle,wert,10);
			//uart_puts(wert);
			//uart_puts("\r\n");
		}
	}
}

//Entfernt die Sanduhr
void clearHourglass() {
	if(hourglassOnScan == 1) {
		OLED_ClearIcon(39,30);
		} else {
		OLED_ClearIcon(39,78);
	}
}

//Reaktion auf den Klick des Encoders
void buttonPress() {
	OLED_ClearIcon(1,30 + (selectedLine * 16));
	//OLED_ReadMemIcon(6);
	//OLED_DispImage(iconBuffer,1,30 + (selectedLine * 16),12,10);
	switch(selectedLine) {
		//Scan soll durchgefuehrt werden
		case 0:
			selectedLine++;
			//Hier den Scan abwarten
			hourglassOnScan = 1;
			_TIM0_INT_en;
			_RXCIE_INT_en;
			triggerOn;
			_delay_ms(1);
			Trigeroff;
			OLED_ClearIcon(39,78);
			for(uint8_t k=0; k<13; k++) {
				OLED_ClearLetter(0 + (3*k),113);
			}
			//char retString[20];
			//uart_gets(retString,14);
			while(uart_str_complete == 0) {
			}
			char zahl;
			for(uint8_t k=0; k<uart_str_len-1; k++) {
				zahl = uart_string[k] - 48;
				OLED_ReadMemLetter(zahl + 56);
				OLED_DispImage(letterBuffer,0 + (3*k),113,8,10);
				uart_str_complete = 0;
			}
			_RXCIE_INT_dis;
			_TIM0_INT_dis;
			clearHourglass();
			break;
		//DLD oder FM (Das wird nun durch Drehen entschieden und nicht laenger ueber Klick)
		case 1:
			selectedLine++;
			//changeType();
			break;
		//POTI Wert(Das wird nun durch Drehen entschieden und nicht laenger ueber Klick)
		case 2:
			selectedLine++;
			//changePoti();
			break;
		//Messung starten
		case 3:
			selectedLine = 0;
			OLED_ClearIcon(39,30);
			hourglassOnScan = 0;
			_TIM0_INT_en;
			//Als erstes Umgebungstemperatur messen
			measureTemp();
			//Dann die Messung vom Geraet starten
			
			//schwelle_erkannt: anzahl der messdurchläufe
			//sensor: art des sensor, also fm, hpd oder dld, benötigt für abfrage für schwelle
			//kalib: kalibriermodus ja = 1 oder nein = 0
			//kalib_summe: wo kalibrierter wert im eeprom abgespeichert werden soll
			//data: zur auswahl der leuchtquelle und einstellung der lichstärke. standardmäßig ist die stärke 0
			//void messung (int schwelle_erkannt, int sensor, int kalib, int kalib_summe, int16_t data)
			messung(10, (selectedType * 2), 0, 0, 0xb000);
			_TIM0_INT_dis;
			clearHourglass();
			break;
		default:
			break;
	}
	
	arrowCounter = 0;
	showArrow = 1;
}

//Animiert die Sanduhr
void updateHourglass() {
	switch(hourglassAnimCounter) {
	case 0:
		OLED_ReadMemIcon(7);
		break;
	case 1:
		OLED_ReadMemIcon(8);
		break;
	case 2:
		OLED_ReadMemIcon(9);
		break;
	case 3:
		OLED_ReadMemIcon(10);
		break;
	default:
		break;	
	}
	if(hourglassOnScan == 1) {
		OLED_DispImage(iconBuffer,39,30,12,10);
	} else {
		OLED_DispImage(iconBuffer,39,78,12,10);
	}
	hourglassAnimCounter++;
	if(hourglassAnimCounter > 3) {
		hourglassAnimCounter = 0;
	}
}

//Interrupt fuer die Sanduhr-Animation
ISR (TIMER1_OVF_vect) {
	updateHourglass();
	TCNT1 = 0xCF2C;
}

//Interrupt fuer den Barcodescanner
ISR (USART_RXC_vect) {
  unsigned char nextChar;

  // Daten aus dem Puffer lesen
  nextChar = UDR;
  if( uart_str_complete == 0 ) {	// wenn uart_string gerade in Verwendung, neues Zeichen verwerfen

	  // Daten werden erst in uart_string geschrieben, wenn nicht String-Ende/max Zeichenlänge erreicht ist/string gerade verarbeitet wird
	  if( nextChar != UART_TERMINATOR && uart_str_count < UART_MAXSTRLEN )
	  {
		  uart_string[uart_str_count] = nextChar;
		  uart_str_count++;
	  }
	  else
	  {
		  uart_string[uart_str_count] = '\0';
		  uart_str_len = uart_str_count;
		  uart_str_count = 0;
		  uart_str_complete = 1;
	  }
  }
}

int main(void)
{
	//Ein und Ausgaenge der Register definieren 0 = Eingang
	reg_ddr |= (1 << reg_data) | (1 << reg_sclk) | (1 << reg_rclk) | (1 << reg_dc);	//Display
	DDRD = 0b00010100; 
	DDRC = 0b00110000;
	DDRB |= (1 << PB2);	//Infrarot LED fuer Messung
	DDRB |= (1 << PB0);	//Infrarot LED fuer Messung
	DDRB |= (1 << PB1);	//Infrarot LED fuer Messung
	
	//adc initialisierungen
	ADCSRA = (1<<ADEN);
	ADMUX = (1<<REFS1) | (1<<REFS0);
	ADMUX |= 7;
	
	// Internen Pullup-Resistor auf dem Input-Pin einschalten
	BUTTON_PORT |= BUTTON_MASK_1;			//Encoder-Press
	BUTTON_PORT |= BUTTON_MASK_2;			//Pin um Richtung zu erkennen
	BUTTON_PORT |= BUTTON_MASK_0;			//Pin um Richtung zu erkennen
	volatile uint8_t encoderDirection = 0;	//Die von "read_encoder_direction" ausgelesene Richtung
	volatile uint8_t encoderFinished = 1;	//Bit zum Debouncen der Encoderdrehung
	
	// Display ansteuern
	OLED_Reset();		//Pin-Reset
	OLED_Init();		//Display initialisieren
	_delay_ms(10);
	OLED_ClearScreen();	//Bildschirm-RAM leeren
	OLED_Command(0xAF);	//Display einschalten
	//OLED_DebugLayout
	OLED_Layout();		//Menue zeigen
	
	//Startkonfiguration einstellen
	selectedLine = 0;
	selectedType = 0;
	selectedPoti = 1;
	updateType = 1;
	updatePoti = 1;
	showArrow = 1;
	showHourglass = 1;
	arrowCounter = 0;
	hourglassCounter = 0;
	hourglassAnimCounter = 0;
	hourglassOnScan = 0;

	measureTemp();

	//TODO: Diesen Wert spaeter entfernen und anfangs leer lassen, weil noch nicht gemessen wurde
	char testWert[3];
	testWert[0] = '4';
	testWert[1] = '8';
	testWert[2] = '9';
	updateMesswert(testWert);

	uart_init();

	// Overflow Interrupt erlauben
	TCCR1B |= (1<<CS12);	// Prescaler 256
	TCNT1 = 0xCF2C;
	
	sei();
	while (1) 
    {
		//Druck auf den Button handlen
		debounce();
		if (button_down)
		{
			button_down = 0;
			buttonPress();
		}
		
		//Drehung des Encoders handlen
		encoderDirection = read_encoder_direction();
		//Die Logik bemerkt den Wechsel von einem der Pins und reagiert entsprechend.
		//Dann wartet Sie durch das "encoderFinished"-Bit, bis der Encoder einrastet, bevor wieder eine Drehung ueberprueft wird
		if(encoderDirection == 0 && encoderFinished == 0) {
			encoderFinished = 1;
		}else if(encoderDirection == 1 && encoderFinished == 1)
		{
			encoderFinished = 0;
			buttonRotation(1);
		} else if (encoderDirection == 2 && encoderFinished == 1) {
			encoderFinished = 0;
			buttonRotation(0);
		}
		
		//DLD oder FM anzeigen
		if(updateType == 1) {
			if(selectedType == 0) {
				//DLD
				OLED_ReadMemLetter(4);		//D
				OLED_DispImage(letterBuffer,0,5,8,10);
				OLED_ReadMemLetter(12);		//L
				OLED_DispImage(letterBuffer,3,5,8,10);
				OLED_ReadMemLetter(4);		//D
				OLED_DispImage(letterBuffer,6,5,8,10);
			} else {
				//FM
				OLED_ReadMemLetter(6);		//F
				OLED_DispImage(letterBuffer,0,5,8,10);
				OLED_ReadMemLetter(13);		//M
				OLED_DispImage(letterBuffer,3,5,8,10);
				OLED_ClearLetter(6,5);
			}
			updateType = 0;
		}
		
		//Potiwert anzeigen
		if(updatePoti == 1) {
			switch(selectedPoti) {
				case 0:
					OLED_ReadMemLetter(56);	//0
					OLED_DispImage(letterBuffer,30,5,8,10);
					OLED_ClearLetter(33,5);
					break;
				case 1:
					OLED_ReadMemLetter(57);	//1
					OLED_DispImage(letterBuffer,30,5,8,10);
					OLED_ClearLetter(33,5);
					break;
				case 2:
					OLED_ReadMemLetter(58);	//2
					OLED_DispImage(letterBuffer,30,5,8,10);
					OLED_ClearLetter(33,5);
					break;
				case 3:
					OLED_ReadMemLetter(59);	//3
					OLED_DispImage(letterBuffer,30,5,8,10);
					OLED_ClearLetter(33,5);
					break;
				case 4:
					OLED_ReadMemLetter(60);	//4
					OLED_DispImage(letterBuffer,30,5,8,10);
					OLED_ClearLetter(33,5);
					break;
				case 5:
					OLED_ReadMemLetter(61);	//5
					OLED_DispImage(letterBuffer,30,5,8,10);
					OLED_ClearLetter(33,5);
					break;
				case 6:
					OLED_ReadMemLetter(62);	//6
					OLED_DispImage(letterBuffer,30,5,8,10);
					OLED_ClearLetter(33,5);
					break;
				case 7:
					OLED_ReadMemLetter(63);	//7
					OLED_DispImage(letterBuffer,30,5,8,10);
					OLED_ClearLetter(33,5);
					break;
				case 8:
					OLED_ReadMemLetter(64);	//8
					OLED_DispImage(letterBuffer,30,5,8,10);
					OLED_ClearLetter(33,5);
					break;
				case 9:
					OLED_ReadMemLetter(65);	//9
					OLED_DispImage(letterBuffer,30,5,8,10);
					OLED_ClearLetter(33,5);
					break;
				case 10:
					OLED_ReadMemLetter(57);	//1
					OLED_DispImage(letterBuffer,30,5,8,10);
					OLED_ReadMemLetter(56);	//0
					OLED_DispImage(letterBuffer,33,5,8,10);
					break;
				default:
					break;
			}
			updatePoti = 0;
		}
		
		//Auswahlpfeil anzeigen, bzw. blinken lassen
		arrowCounter++;
		if (arrowCounter > 200) {
			arrowCounter = 0;
			if(showArrow == 1) {
				showArrow = 0;
			} else {
				showArrow = 1;
			}
		}
		if (showArrow == 1) {
			OLED_ReadMemIcon(1);
			OLED_DispImage(iconBuffer,1,30 + (selectedLine * 16),12,10);
		} else {
			OLED_ClearIcon(1, 30 + (selectedLine * 16));
		}
    }
}