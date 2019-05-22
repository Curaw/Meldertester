
/*
 * displayDriver.c
 *
 * Created: 19.04.2019 16:54:36
 * Author : Daniel Kison
 */ 

#include "gfx.c"

//Display
#define reg_port PORTB
#define reg_data PB3
#define reg_rclk PB6
#define reg_sclk PB5
#define reg_dc	 PB4
#define reg_ddr DDRB
#define RST_IDLE 	PORTD |= (1<<PC3)
#define RST_ACTIVE 	PORTD &= ~(1<<PC3)

uint8_t letterBuffer[40];
uint8_t iconBuffer[60];
uint8_t feedbackCode;
uint32_t measuredValue;

void OLED_Reset()
{
	//Hard Reset
	RST_IDLE;
	_delay_ms(1);
	RST_ACTIVE;
	_delay_ms(10);
	RST_IDLE;
	_delay_ms(1000);
}

//Schick ein Command an das Display. Alle Commands stehen im SSD1327 Datasheet. Dieser Code wurde von Sebastian provided.
void OLED_Command(uint8_t data)
{
	reg_port &= ~(1<<reg_dc);	// Command senden
	reg_port &= ~(1<<reg_rclk);	// SS Tick von 1 auf 0
	for (int i=0; i<8;i++)
	{
		
		reg_port &= ~(1<<reg_data);	// SER auf 0
		if(data & 0x80) // Wenn erstes Bit = 1 dann ...
		{
			reg_port |= (1<<reg_data); // ...SER auf 1
		}
		data <<= 1; // Daten um ein Bit nach links schieben
		reg_port &= ~(1<<reg_sclk); // SLK Tick von 1 auf 0 auf 1
		reg_port |= (1<<reg_sclk);
	}
	reg_port |= (1<<reg_rclk);
}

//Schick Daten an das Display. Dieser Code wurde von Sebastian provided.
void OLED_Data(uint8_t data)
{
	reg_port |= (1<<reg_dc);	// Data senden
	reg_port &= ~(1<<reg_rclk);	// SS Tick von 1 auf 0
	for (int i=0; i<8;i++)
	{
		
		reg_port &= ~(1<<reg_data);	// SER auf 0
		if(data & 0x80)				// Wenn erstes Bit = 1 dann ...
		{
			reg_port |= (1<<reg_data); // ...SER auf 1
		}
		data <<= 1;					//Daten um ein Bit nach links schieben
		reg_port &= ~(1<<reg_sclk);	// SLK Tick von 1 auf 0 auf 1
		reg_port |= (1<<reg_sclk);
	}
	reg_port |= (1<<reg_rclk);
}

static void OLED_Init(void)
{
	OLED_Command(0xae);	//--turn off oled panel

	OLED_Command(0x15);	//set column address
	OLED_Command(0x00);	//start column   0
	OLED_Command(0x7f);	//end column   127

	OLED_Command(0x75);	//set row address
	OLED_Command(0x00);	//start row   0
	OLED_Command(0x7f);	//end row   127

	OLED_Command(0x81);	//set contrast control
	OLED_Command(0x80);

	OLED_Command(0xa0);	//gment remap
	OLED_Command(0x51);	//51

	OLED_Command(0xa1);	//start line
	OLED_Command(0x00);

	OLED_Command(0xa2);	//display offset
	OLED_Command(0x00);

	OLED_Command(0xa4);	//rmal display
	OLED_Command(0xa8);	//set multiplex ratio
	OLED_Command(0x7f);

	OLED_Command(0xb1);	//set phase leghth
	OLED_Command(0xf1);

	OLED_Command(0xb3);	//set dclk
	OLED_Command(0x00);	//80Hz:0xc1 90Hz:0xe1   100Hz:0x00   110Hz:0x30 120Hz:0x50   130Hz:0x70     01

	OLED_Command(0xab);	//
	OLED_Command(0x01);	//

	OLED_Command(0xb6);	//set phase leghth
	OLED_Command(0x0f);

	OLED_Command(0xbe);
	OLED_Command(0x0f);

	OLED_Command(0xbc);
	OLED_Command(0x08);

	OLED_Command(0xd5);
	OLED_Command(0x62);

	OLED_Command(0xfd);
	OLED_Command(0x12);

}

//Legt fest, in welchem Bereich des Displays die kommenden Daten eingetragen werden sollen
//Kann man sich vorstellen, als wuerde man einen Teil des Displays rausschneiden,
//dann von oben links nach rechts unten diesen Teil mit Daten befuellen und wieder reinpacken
void OLED_SetWindow(uint8_t Xstart, uint8_t Xend, uint8_t Ystart, uint8_t Yend)
{
	OLED_Command(0x75);		//Reihen (Maximal 127)
	OLED_Command(Ystart);
	OLED_Command(Yend - 1);
	
	OLED_Command(0x15);		//Spalten (Maximal 63, weil zwei Pixel von einem Byte repraesentiert werden)
	OLED_Command(Xstart);
	OLED_Command(Xend - 1);
}

//Display leeren
void OLED_ClearScreen()
{
	unsigned int i, m;
	OLED_SetWindow(0, 0, 128, 128);
	for (i = 0; i < 128; i++) {
		for (m = 0; m < (128 / 2); m++) {
			OLED_Data(0);
		}
	}
}

//Zeichnet eine horizontale Linie auf Hoehe yPos
void OLED_HLine(uint8_t yPos) {  
	
	if(yPos > 127) {
		yPos = 127;
	}
	OLED_SetWindow(0,64,yPos,yPos + 1);
	
	for (uint8_t i = 0; i < 63; i++)
	{
		OLED_Data(0xff);
	}
}

//Zeichnet die im Buffer liegende Grafik
void OLED_DispImage(unsigned char *img, uint8_t xPos, uint8_t yPos, uint8_t width, uint8_t height) {
	width = width/2;
	OLED_SetWindow(xPos,xPos + width,yPos,yPos + height);
	for (uint8_t i = 0; i < height*width; i++)
	{
		OLED_Data(img[i]);
	}
}

//Liest die einzelnen Buchstaben aus dem Programmspeicher, da der RAM des Mikrocontrollers viel zu klein ist
void OLED_ReadMemLetter(uint8_t letterNumber) {
	char myChar;
	uint8_t *addressPointer;
	
	switch(letterNumber) {
		case 1:
			addressPointer = &(letterA);
			break;
		case 2:
			addressPointer = &(letterB);
			break;
		case 3:
			addressPointer = &(letterC);
			break;
		case 4:
			addressPointer = &(letterD);
			break;
		case 5:
			addressPointer = &(letterE);
		break;
		case 6:
			addressPointer = &(letterF);
			break;
		case 7:
			addressPointer = &(letterG);
			break;
		case 8:
			addressPointer = &(letterH);
			break;
		case 9:
			addressPointer = &(letterI);
			break;
		case 10:
			addressPointer = &(letterJ);
			break;
		case 11:
			addressPointer = &(letterK);
			break;
		case 12:
			addressPointer = &(letterL);
			break;
		case 13:
			addressPointer = &(letterM);
			break;
		case 14:
			addressPointer = &(letterN);
			break;
		case 15:
			addressPointer = &(letterO);
			break;
		case 16:
			addressPointer = &(letterP);
			break;
		case 17:
			addressPointer = &(letterQ);
			break;
		case 18:
			addressPointer = &(letterR);
			break;
		case 19:
			addressPointer = &(letterS);
			break;
		case 20:
			addressPointer = &(letterT);
			break;
		case 21:
			addressPointer = &(letterU);
			break;
		case 22:
			addressPointer = &(letterV);
			break;
		case 23:
			addressPointer = &(letterW);
			break;
		case 24:
			addressPointer = &(letterX);
			break;
		case 25:
			addressPointer = &(letterY);
			break;
		case 26:
			addressPointer = &(letterZ);
			break;
		case 27:
			addressPointer = &(smallA);
			break;
		case 28:
			addressPointer = &(smallB);
			break;
		case 29:
			addressPointer = &(smallC);
			break;
		case 30:
			addressPointer = &(smallD);
			break;
		case 31:
			addressPointer = &(smallE);
			break;
		case 32:
			addressPointer = &(smallF);
			break;
		case 33:
			addressPointer = &(smallG);
			break;
		case 34:
			addressPointer = &(smallH);
			break;
		case 35:
			addressPointer = &(smallI);
			break;
		case 36:
			addressPointer = &(smallJ);
			break;
		case 37:
			addressPointer = &(smallK);
			break;
		case 38:
			addressPointer = &(smallL);
			break;
		case 39:
			addressPointer = &(smallM);
			break;
		case 40:
			addressPointer = &(smallN);
			break;
		case 41:
			addressPointer = &(smallO);
			break;
		case 42:
			addressPointer = &(smallP);
			break;
		case 43:
			addressPointer = &(smallQ);
			break;
		case 44:
			addressPointer = &(smallR);
			break;
		case 45:
			addressPointer = &(smallS);
			break;
		case 46:
			addressPointer = &(smallT);
			break;
		case 47:
			addressPointer = &(smallU);
			break;
		case 48:
			addressPointer = &(smallV);
			break;
		case 49:
			addressPointer = &(smallW);
			break;
		case 50:
			addressPointer = &(smallX);
			break;
		case 51:
			addressPointer = &(smallY);
			break;
		case 52:
			addressPointer = &(smallZ);
			break;
		case 53:
			addressPointer = &(letterDegree);
			break;
		case 54:
			addressPointer = &(letterColon);
			break;
		case 55:
			addressPointer = &(letterDot);
			break;
		case 56:
			addressPointer = &(zero);
			break;		
		case 57:
			addressPointer = &(one);
			break;
		case 58:
			addressPointer = &(two);
			break;
		case 59:
			addressPointer = &(three);
			break;
		case 60:
			addressPointer = &(four);
			break;
		case 61:
			addressPointer = &(five);
			break;
		case 62:
			addressPointer = &(six);
			break;
		case 63:
			addressPointer = &(seven);
			break;
		case 64:
			addressPointer = &(eight);
			break;
		case 65:
			addressPointer = &(nine);
			break;
		case 66:
			addressPointer = &(letterClear);
			break;
		case 67:
			addressPointer = &(letterQuestion);
			break;
		default:
			break;
	}
	
	for (uint8_t k = 0; k < 40; k++) {
		myChar = pgm_read_byte(addressPointer + k);
		letterBuffer[k] = myChar;
	}
}

//Liest die einzelnen Icons aus dem Programmspeicher, da der RAM des Mikrocontrollers viel zu klein ist
//TODO: Das hier muss eigtl dynamisch in OLED_ReadMemLetter passieren, aber Zeitdruck.
void OLED_ReadMemIcon(uint8_t letterNumber) {
	char myChar;
	uint8_t *addressPointer;
	
	switch(letterNumber) {
		case 1:
			addressPointer = &(arrow);
			break;
		case 2:
			addressPointer = &(batteryFull);
			break;
		case 3:
			addressPointer = &(batteryGood);
			break;
		case 4:
			addressPointer = &(batteryLow);
			break;
		case 5:
			addressPointer = &(batteryEmpty);
			break;
		case 6:
			addressPointer = &(clear);
			break;
		case 7:
			addressPointer = &(wait1);
			break;
		case 8:
			addressPointer = &(wait2);
			break;
		case 9:
			addressPointer = &(wait3);
			break;
		case 10:
			addressPointer = &(wait4);
			break;
		case 11:
			addressPointer = &(good);
			break;
		case 12:
			addressPointer = &(bad);
			break;
		default:
		break;
	}
	
	for (uint8_t k = 0; k < 60; k++) {
		myChar = pgm_read_byte(addressPointer + k);
		iconBuffer[k] = myChar;
	}
}

//Loescht den Buchstaben an der Stelle (xPos, yPos)
void OLED_ClearLetter(uint8_t xPos, uint8_t yPos) {
	OLED_ReadMemLetter(66);
	OLED_DispImage(letterBuffer,xPos,yPos,8,10);
}

//Loescht das Icon an der Stelle (xPos, yPos)
void OLED_ClearIcon(uint8_t xPos, uint8_t yPos) {
	OLED_ReadMemIcon(6);
	OLED_DispImage(iconBuffer,xPos,yPos,12,10);	
}

//Zeichnet einen angegebenen String
//TODO Das hier funktioniert bisher nur fuer gross- und kleinBuchstaben
void OLED_ShowString(char textToShow[], uint8_t xPos, uint8_t yPos) {
	for(int i = 0; textToShow[i] != '\0'; ++i) {
		if(xPos + (i*2) >= 63) {
			return;
		}
		if(textToShow[i] > 90) {
			OLED_ReadMemLetter((textToShow[i] - 70 ));	
			OLED_DispImage(letterBuffer,xPos + (i*4),yPos + 1,8,10);
		} else {
			OLED_ReadMemLetter((textToShow[i] - 64 ));
			OLED_DispImage(letterBuffer,xPos + (i*4),yPos,8,10);
		}
	}
}

void showMesswert() {
	//Konvertierung des Wertes in einen integer
	//int firstNumber = newVal[0] - '0';
	//int secondNumber = newVal[1] - '0';
	//int thirdNumber = newVal[2] - '0';
	
	uint8_t thirdNumber = measuredValue % 10;
	measuredValue = measuredValue / 10;
	uint8_t secondNumber = measuredValue % 10;
	measuredValue = measuredValue / 10;
	uint8_t firstNumber = measuredValue % 10;
	
	OLED_ReadMemLetter(feedbackCode);
	OLED_DispImage(letterBuffer,12,46,8,10);
	
	OLED_ReadMemLetter(13);
	OLED_DispImage(letterBuffer,39,46,8,10);
	OLED_ReadMemLetter(54);
	OLED_DispImage(letterBuffer,43,46,8,10);
	OLED_ReadMemLetter(firstNumber + 56);
	OLED_DispImage(letterBuffer,46,46,8,10);
	OLED_ReadMemLetter(secondNumber + 56);
	OLED_DispImage(letterBuffer,49,46,8,10);
	OLED_ReadMemLetter(thirdNumber + 56);
	OLED_DispImage(letterBuffer,52,46,8,10);
	
	//Referenz
	OLED_ReadMemLetter(18);
	OLED_DispImage(letterBuffer,39,62,8,10);
	OLED_ReadMemLetter(54);
	OLED_DispImage(letterBuffer,43,62,8,10);
	OLED_ReadMemLetter(64);
	OLED_DispImage(letterBuffer,46,62,8,10);
	OLED_ReadMemLetter(56);
	OLED_DispImage(letterBuffer,49,62,8,10);
	OLED_ReadMemLetter(56);
	OLED_DispImage(letterBuffer,52,62,8,10);
	//OLED_ReadMemLetter(53);
	//OLED_DispImage(letterBuffer,55,62,8,10);
	//OLED_ReadMemLetter(3);
	//OLED_DispImage(letterBuffer,58,62,8,10);

}


//Zeichnet das Menu fuer das Projekt
void OLED_Layout(uint8_t menuToShow) {
	
	OLED_ClearScreen();
	OLED_HLine(0);
	OLED_HLine(127);
	if(menuToShow == 1) {
		OLED_ShowString("Measurement", 10, 16);
		OLED_ShowString("complete", 16, 27);
		showMesswert();
		return;
	} else if(menuToShow == 2) {
		OLED_ShowString("Export", 18, 16);
		OLED_ReadMemLetter(67);		//?
		OLED_DispImage(letterBuffer,43,16,8,10);
		
		OLED_ShowString("Yes", 28, 32);
		OLED_ShowString("No", 28, 49);
	} else {
		//DLD
		OLED_ReadMemLetter(4);		//D
		OLED_DispImage(letterBuffer,0,5,8,10);
		OLED_ReadMemLetter(12);		//L
		OLED_DispImage(letterBuffer,3,5,8,10);
		OLED_ReadMemLetter(4);		//D
		OLED_DispImage(letterBuffer,6,5,8,10);
		
		//Poti
		OLED_ReadMemLetter(16);
		OLED_DispImage(letterBuffer,15,5,8,10);
		OLED_ReadMemLetter(41);
		OLED_DispImage(letterBuffer,18,6,8,10);
		OLED_ReadMemLetter(46);
		OLED_DispImage(letterBuffer,21,6,8,10);
		OLED_ReadMemLetter(35);
		OLED_DispImage(letterBuffer,24,6,8,10);
		OLED_ReadMemLetter(54);
		OLED_DispImage(letterBuffer,27,5,8,10);

		//Temperatur
		OLED_ReadMemLetter(53);
		OLED_DispImage(letterBuffer,45,5,8,10);
		OLED_ReadMemLetter(3);
		OLED_DispImage(letterBuffer,48,5,8,10);

		//Akku
		OLED_ReadMemIcon(3);
		OLED_DispImage(iconBuffer,55,4,12,10);
	
		//Scan ID
		OLED_ReadMemLetter(57);
		OLED_DispImage(letterBuffer,10,30,8,10);
		OLED_ReadMemLetter(55);
		OLED_DispImage(letterBuffer,13,30,8,10);
		OLED_ReadMemLetter(19);
		OLED_DispImage(letterBuffer,16,30,8,10);
		OLED_ReadMemLetter(29);
		OLED_DispImage(letterBuffer,19,31,8,10);
		OLED_ReadMemLetter(27);
		OLED_DispImage(letterBuffer,21,31,8,10);
		OLED_ReadMemLetter(40);
		OLED_DispImage(letterBuffer,25,31,8,10);
		OLED_ReadMemLetter(9);
		OLED_DispImage(letterBuffer,31,31,8,10);
		OLED_ReadMemLetter(4);
		OLED_DispImage(letterBuffer,35,31,8,10);
	
		//Scan SN
		OLED_ReadMemLetter(58);
		OLED_DispImage(letterBuffer,10,46,8,10);
		OLED_ReadMemLetter(55);
		OLED_DispImage(letterBuffer,13,46,8,10);
		OLED_ReadMemLetter(19);
		OLED_DispImage(letterBuffer,16,46,8,10);
		OLED_ReadMemLetter(29);
		OLED_DispImage(letterBuffer,19,47,8,10);
		OLED_ReadMemLetter(27);
		OLED_DispImage(letterBuffer,21,47,8,10);
		OLED_ReadMemLetter(40);
		OLED_DispImage(letterBuffer,25,47,8,10);
		OLED_ReadMemLetter(19);
		OLED_DispImage(letterBuffer,31,47,8,10);
		OLED_ReadMemLetter(14);
		OLED_DispImage(letterBuffer,35,47,8,10);

		//OLED_ReadMemLetter(58);
		//OLED_DispImage(letterBuffer,10,46,8,10);
		//OLED_ReadMemLetter(55);
		//OLED_DispImage(letterBuffer,13,46,8,10);
		//OLED_ReadMemLetter(20);
		//OLED_DispImage(letterBuffer,16,46,8,10);
		//OLED_ReadMemLetter(51);
		//OLED_DispImage(letterBuffer,19,47,8,10);
		//OLED_ReadMemLetter(42);
		//OLED_DispImage(letterBuffer,22,47,8,10);
		//OLED_ReadMemLetter(31);
		//OLED_DispImage(letterBuffer,26,47,8,10);
	
		//Messung
		//OLED_ReadMemLetter(13);
		//OLED_DispImage(letterBuffer,39,46,8,10);
		//OLED_ReadMemLetter(54);
		//OLED_DispImage(letterBuffer,43,46,8,10);
		//Ab hier Grad Celsius
		//OLED_ReadMemLetter(53);
		//OLED_DispImage(letterBuffer,55,46,8,10);
		//OLED_ReadMemLetter(3);
		//OLED_DispImage(letterBuffer,58,46,8,10);

		//Poti
		OLED_ReadMemLetter(59);
		OLED_DispImage(letterBuffer,10,62,8,10);
		OLED_ReadMemLetter(55);
		OLED_DispImage(letterBuffer,13,62,8,10);
		OLED_ReadMemLetter(16);
		OLED_DispImage(letterBuffer,16,62,8,10);
		OLED_ReadMemLetter(41);
		OLED_DispImage(letterBuffer,19,63,8,10);
		OLED_ReadMemLetter(46);
		OLED_DispImage(letterBuffer,22,63,8,10);
		OLED_ReadMemLetter(35);
		OLED_DispImage(letterBuffer,26,63,8,10);

		////Referenz
		//OLED_ReadMemLetter(18);
		//OLED_DispImage(letterBuffer,39,62,8,10);
		//OLED_ReadMemLetter(54);
		//OLED_DispImage(letterBuffer,43,62,8,10);
		//OLED_ReadMemLetter(58);
		//OLED_DispImage(letterBuffer,46,62,8,10);
		//OLED_ReadMemLetter(56);
		//OLED_DispImage(letterBuffer,49,62,8,10);
		//OLED_ReadMemLetter(56);
		//OLED_DispImage(letterBuffer,52,62,8,10);
		//OLED_ReadMemLetter(53);
		//OLED_DispImage(letterBuffer,55,62,8,10);
		//OLED_ReadMemLetter(3);
		//OLED_DispImage(letterBuffer,58,62,8,10);

		//Start
		OLED_ReadMemLetter(60);
		OLED_DispImage(letterBuffer,10,78,8,10);
		OLED_ReadMemLetter(55);
		OLED_DispImage(letterBuffer,13,78,8,10);
		OLED_ReadMemLetter(19);
		OLED_DispImage(letterBuffer,16,78,8,10);
		OLED_ReadMemLetter(46);
		OLED_DispImage(letterBuffer,19,79,8,10);
		OLED_ReadMemLetter(27);
		OLED_DispImage(letterBuffer,22,79,8,10);
		OLED_ReadMemLetter(44);
		OLED_DispImage(letterBuffer,25,79,8,10);
		OLED_ReadMemLetter(46);
		OLED_DispImage(letterBuffer,28,79,8,10);

		//Smiley
		//OLED_ReadMemIcon(11);
		//OLED_DispImage(iconBuffer,55,113,12,10);
		//OLED_ReadMemIcon(12);
		//OLED_DispImage(iconBuffer,48,113,12,10);		
	}
}

//Zeichnet ein DebugLayout mit allen Symbolen
void OLED_DebugLayout() {

	OLED_HLine(0);
	OLED_HLine(127);
	
	OLED_ReadMemLetter(1);
	OLED_DispImage(letterBuffer,0,5,8,10);
	OLED_ReadMemLetter(2);
	OLED_DispImage(letterBuffer,4,5,8,10);
	OLED_ReadMemLetter(3);
	OLED_DispImage(letterBuffer,8,5,8,10);
	OLED_ReadMemLetter(4);
	OLED_DispImage(letterBuffer,12,5,8,10);
	OLED_ReadMemLetter(5);
	OLED_DispImage(letterBuffer,16,5,8,10);
	OLED_ReadMemLetter(6);
	OLED_DispImage(letterBuffer,20,5,8,10);
	OLED_ReadMemLetter(7);
	OLED_DispImage(letterBuffer,24,5,8,10);
	OLED_ReadMemLetter(8);
	OLED_DispImage(letterBuffer,28,5,8,10);
	OLED_ReadMemLetter(9);
	OLED_DispImage(letterBuffer,32,5,8,10);
	OLED_ReadMemLetter(10);
	OLED_DispImage(letterBuffer,36,5,8,10);
	OLED_ReadMemLetter(11);
	OLED_DispImage(letterBuffer,40,5,8,10);
	OLED_ReadMemLetter(12);
	OLED_DispImage(letterBuffer,44,5,8,10);
	OLED_ReadMemLetter(13);
	OLED_DispImage(letterBuffer,48,5,8,10);
	OLED_ReadMemLetter(14);
	OLED_DispImage(letterBuffer,52,5,8,10);
	OLED_ReadMemLetter(15);
	OLED_DispImage(letterBuffer,0,15,8,10);
	OLED_ReadMemLetter(16);
	OLED_DispImage(letterBuffer,4,15,8,10);
	OLED_ReadMemLetter(17);
	OLED_DispImage(letterBuffer,8,15,8,10);
	OLED_ReadMemLetter(18);
	OLED_DispImage(letterBuffer,12,15,8,10);
	OLED_ReadMemLetter(19);
	OLED_DispImage(letterBuffer,16,15,8,10);
	OLED_ReadMemLetter(20);
	OLED_DispImage(letterBuffer,20,15,8,10);
	OLED_ReadMemLetter(21);
	OLED_DispImage(letterBuffer,24,15,8,10);
	OLED_ReadMemLetter(22);
	OLED_DispImage(letterBuffer,28,15,8,10);
	OLED_ReadMemLetter(23);
	OLED_DispImage(letterBuffer,32,15,8,10);
	OLED_ReadMemLetter(24);
	OLED_DispImage(letterBuffer,36,15,8,10);
	OLED_ReadMemLetter(25);
	OLED_DispImage(letterBuffer,40,15,8,10);
	OLED_ReadMemLetter(26);
	OLED_DispImage(letterBuffer,44,15,8,10);

	//Kleine Buchstaben
	OLED_ReadMemLetter(27);
	OLED_DispImage(letterBuffer,0,30,8,10);
	OLED_ReadMemLetter(28);
	OLED_DispImage(letterBuffer,4,30,8,10);
	OLED_ReadMemLetter(29);
	OLED_DispImage(letterBuffer,8,30,8,10);
	OLED_ReadMemLetter(30);
	OLED_DispImage(letterBuffer,12,30,8,10);
	OLED_ReadMemLetter(31);
	OLED_DispImage(letterBuffer,16,30,8,10);
	OLED_ReadMemLetter(32);
	OLED_DispImage(letterBuffer,20,30,8,10);
	OLED_ReadMemLetter(33);
	OLED_DispImage(letterBuffer,24,30,8,10);
	OLED_ReadMemLetter(34);
	OLED_DispImage(letterBuffer,28,30,8,10);
	OLED_ReadMemLetter(35);
	OLED_DispImage(letterBuffer,32,30,8,10);
	OLED_ReadMemLetter(36);
	OLED_DispImage(letterBuffer,36,30,8,10);
	OLED_ReadMemLetter(37);
	OLED_DispImage(letterBuffer,40,30,8,10);
	OLED_ReadMemLetter(38);
	OLED_DispImage(letterBuffer,44,30,8,10);
	OLED_ReadMemLetter(39);
	OLED_DispImage(letterBuffer,48,30,8,10);
	OLED_ReadMemLetter(40);
	OLED_DispImage(letterBuffer,52,30,8,10);
	OLED_ReadMemLetter(41);
	OLED_DispImage(letterBuffer,0,40,8,10);
	OLED_ReadMemLetter(42);
	OLED_DispImage(letterBuffer,4,40,8,10);
	OLED_ReadMemLetter(43);
	OLED_DispImage(letterBuffer,8,40,8,10);
	OLED_ReadMemLetter(44);
	OLED_DispImage(letterBuffer,12,40,8,10);
	OLED_ReadMemLetter(45);
	OLED_DispImage(letterBuffer,16,40,8,10);
	OLED_ReadMemLetter(46);
	OLED_DispImage(letterBuffer,20,40,8,10);
	OLED_ReadMemLetter(47);
	OLED_DispImage(letterBuffer,24,40,8,10);
	OLED_ReadMemLetter(48);
	OLED_DispImage(letterBuffer,28,40,8,10);
	OLED_ReadMemLetter(49);
	OLED_DispImage(letterBuffer,32,40,8,10);
	OLED_ReadMemLetter(50);
	OLED_DispImage(letterBuffer,36,40,8,10);
	OLED_ReadMemLetter(51);
	OLED_DispImage(letterBuffer,40,40,8,10);
	OLED_ReadMemLetter(52);
	OLED_DispImage(letterBuffer,44,40,8,10);	
	
	//Zahlen und Sonderzeichen
	OLED_ReadMemLetter(53);
	OLED_DispImage(letterBuffer,0,55,8,10);
	OLED_ReadMemLetter(54);
	OLED_DispImage(letterBuffer,4,55,8,10);
	OLED_ReadMemLetter(55);
	OLED_DispImage(letterBuffer,8,55,8,10);
	OLED_ReadMemLetter(56);
	OLED_DispImage(letterBuffer,12,55,8,10);
	OLED_ReadMemLetter(57);
	OLED_DispImage(letterBuffer,16,55,8,10);
	OLED_ReadMemLetter(58);
	OLED_DispImage(letterBuffer,20,55,8,10);
	OLED_ReadMemLetter(59);
	OLED_DispImage(letterBuffer,24,55,8,10);
	OLED_ReadMemLetter(60);
	OLED_DispImage(letterBuffer,28,55,8,10);
	OLED_ReadMemLetter(61);
	OLED_DispImage(letterBuffer,32,55,8,10);
	OLED_ReadMemLetter(62);
	OLED_DispImage(letterBuffer,36,55,8,10);
	OLED_ReadMemLetter(63);
	OLED_DispImage(letterBuffer,40,55,8,10);
	OLED_ReadMemLetter(64);
	OLED_DispImage(letterBuffer,44,55,8,10);

	//Icons
	OLED_ReadMemIcon(1);
	OLED_DispImage(iconBuffer,0,70,10,10);
	OLED_ReadMemIcon(2);
	OLED_DispImage(iconBuffer,6,70,10,10);
	OLED_ReadMemIcon(3);
	OLED_DispImage(iconBuffer,12,70,10,10);
	OLED_ReadMemIcon(4);
	OLED_DispImage(iconBuffer,18,70,10,10);
	OLED_ReadMemIcon(5);
	OLED_DispImage(iconBuffer,24,70,10,10);
}
