
/*
 * gfx.c
 *
 * Grafiken fuer den DisplayDriver
 * Alle Grafiken werden im Programmspeicher abgelegt,
 * weil der RAM des Mikrocontrollers viel zu klein ist
 *
 * Created: 19.04.2019 16:54:36
 * Author : Daniel Kison
 */

static const uint8_t letterA[40] PROGMEM = {
	0x00, 0x00, 0x00, 0x00,
	0x00, 0xff, 0x00, 0x00,
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0xf0, 0xf0, 0x00,
	0x00, 0xf0, 0xf0, 0x00,
	0x00, 0xf0, 0xf0, 0x00,
	0x0f, 0xff, 0xff, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0xff, 0xf0, 0xff, 0xf0,
	0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterB[40] PROGMEM = {
	0x00, 0x00, 0x00, 0x00,
	0xff, 0xff, 0xf0, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0x0f, 0xff, 0xf0, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0xff, 0xff, 0xf0, 0x00,
	0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterC[40] PROGMEM = {
	0x00, 0x00, 0x00, 0x00,
	0x00, 0xff, 0xff, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0x0f, 0x00, 0x00, 0x00,
	0x0f, 0x00, 0x00, 0x00,
	0x0f, 0x00, 0x00, 0x00,
	0x0f, 0x00, 0x00, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0x00, 0xff, 0xf0, 0x00,
	0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterD[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x00, 0x00,
    0x0f, 0x00, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0xf0, 0x00,
    0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterE[40] PROGMEM = {
	0x00, 0x00, 0x00, 0x00,
	0xff, 0xff, 0xff, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0x0f, 0x0f, 0x00, 0x00,
	0x0f, 0xff, 0x00, 0x00,
	0x0f, 0x0f, 0x00, 0x00,
	0x0f, 0x00, 0x00, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0xff, 0xff, 0xff, 0x00,
	0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterF[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x0f, 0xff, 0xff, 0xf0,
    0x00, 0xf0, 0x00, 0xf0,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x0f, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterG[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x00, 0x00,
    0x0f, 0x00, 0x00, 0x00,
    0x0f, 0x00, 0xff, 0xf0,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterH[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterI[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterJ[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x0f, 0x00, 0xf0, 0x00,
    0x0f, 0x00, 0xf0, 0x00,
    0x0f, 0x00, 0xf0, 0x00,
    0x00, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterK[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0xf0, 0x00,
    0x0f, 0x0f, 0x00, 0x00,
    0x0f, 0xff, 0x00, 0x00,
    0x0f, 0x00, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0xff, 0xf0, 0x0f, 0xf0,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterL[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x0f, 0xff, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x0f, 0x00,
    0x00, 0xf0, 0x0f, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterM[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x0f, 0xf0, 0xff, 0x00,
    0x0f, 0xf0, 0xff, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterN[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x0f, 0xf0, 0x0f, 0x00,
    0x0f, 0xf0, 0x0f, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x0f, 0x00, 0xff, 0x00,
    0xff, 0xf0, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterO[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterP[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x0f, 0xff, 0xf0, 0x00,
    0x00, 0xf0, 0x0f, 0x00,
    0x00, 0xf0, 0x0f, 0x00,
    0x00, 0xf0, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x0f, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterQ[40] PROGMEM = {
	0x00, 0x00, 0x00, 0x00,
	0x00, 0xff, 0xf0, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0x0f, 0x00, 0x0f, 0x00,
	0x00, 0xff, 0xf0, 0x00,
	0x00, 0x0f, 0xff, 0x00
};

static const uint8_t letterR[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0xff, 0xf0, 0x00, 0xf0,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterS[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0x0f, 0x00,
    0x0f, 0x00, 0xff, 0x00,
    0x0f, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x0f, 0xf0, 0x0f, 0x00,
    0x0f, 0x0f, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterT[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterU[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterV[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterW[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterX[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0xff, 0x00, 0x0f, 0xf0,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0xff, 0x00, 0x0f, 0xf0,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterY[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterZ[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallA[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xff, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xff, 0xf0,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallB[40] PROGMEM = {
   0xff, 0x00, 0x00, 0x00,
   0x0f, 0x00, 0x00, 0x00,
   0x0f, 0x0f, 0xf0, 0x00,
   0x0f, 0xf0, 0x0f, 0x00,
   0x0f, 0x00, 0x0f, 0x00,
   0x0f, 0x00, 0x0f, 0x00,
   0x0f, 0x00, 0x0f, 0x00,
   0xff, 0xff, 0xf0, 0x00,
   0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallC[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x00, 0x00,
    0x0f, 0x00, 0x00, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallD[40] PROGMEM = {
    0x00, 0x00, 0xff, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0x0f, 0x00,
    0x0f, 0x00, 0xff, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xff, 0xf0,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallE[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x0f, 0x00, 0x00, 0x00,
    0x0f, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallF[40] PROGMEM = {
    0x00, 0x0f, 0xff, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallG[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0x0f, 0xf0,
    0x0f, 0x00, 0xff, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00
};

static const uint8_t smallH[40] PROGMEM = {
    0xff, 0x00, 0x00, 0x00,
    0x0f, 0x00, 0x00, 0x00,
    0x0f, 0x0f, 0xf0, 0x00,
    0x0f, 0xf0, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallI[40] PROGMEM = {
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x0f, 0xff, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallJ[40] PROGMEM = {
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x0f, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x0f, 0xff, 0x00, 0x00
};

static const uint8_t smallK[40] PROGMEM = {
    0x0f, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0xff, 0xf0,
    0x00, 0xf0, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0xf0, 0x0f, 0x00,
    0x0f, 0xf0, 0xff, 0xf0,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallL[40] PROGMEM = {
    0x00, 0xff, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallM[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xf0, 0xf0, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0xff, 0xff, 0xff, 0xf0,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallN[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0x0f, 0xf0, 0x00,
    0x0f, 0xf0, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallO[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallP[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x0f, 0xf0, 0xff, 0x00,
    0x00, 0xff, 0x00, 0xf0,
    0x00, 0xf0, 0x00, 0xf0,
    0x00, 0xf0, 0x00, 0xf0,
    0x00, 0xf0, 0x00, 0xf0,
    0x00, 0xff, 0xff, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x0f, 0xff, 0x00, 0x00
};

static const uint8_t smallQ[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0x0f, 0xf0,
    0x0f, 0x00, 0xff, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0xff, 0xf0
};

static const uint8_t smallR[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x0f, 0xf0, 0xff, 0x00,
    0x00, 0xff, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x0f, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallS[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallT[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0xf0,
    0x00, 0x0f, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallU[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0x00, 0xff, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0xff, 0x00,
    0x00, 0xff, 0x0f, 0xf0,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallV[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallW[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallX[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0x00, 0xff, 0x00,
    0x0f, 0x00, 0xf0, 0x00,
    0x00, 0xff, 0x00, 0x00,
    0x00, 0xff, 0x00, 0x00,
    0x0f, 0x00, 0xf0, 0x00,
    0xff, 0x00, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t smallY[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0xff, 0xf0, 0xff, 0xf0,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xf0, 0x0f, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0x0f, 0xf0, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x0f, 0xff, 0xf0, 0x00
};

static const uint8_t smallZ[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x0f, 0x00, 0xf0, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterDegree[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0xf0, 0x00,
    0x00, 0xf0, 0x0f, 0x00,
    0x00, 0xf0, 0x0f, 0x00,
    0x00, 0x0f, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterColon[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0x00, 0x00,
    0x00, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0x00, 0x00,
    0x00, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterDot[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0x00, 0x00,
    0x00, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t zero[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0xff, 0x00,
    0x0f, 0x0f, 0x0f, 0x00,
    0x0f, 0xf0, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t one[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t two[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t three[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0xf0, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t four[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0xf0, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x00, 0xf0, 0xf0, 0x00,
    0x0f, 0x00, 0xf0, 0x00,
    0xf0, 0x00, 0xf0, 0x00,
    0xff, 0xff, 0xff, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x0f, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t five[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t six[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0xff, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x0f, 0x00, 0x00, 0x00,
    0x0f, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t seven[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x0f, 0xff, 0xff, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t eight[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t nine[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x0f, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterClear[40] PROGMEM = {
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

static const uint8_t letterQuestion[40] PROGMEM = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xf0, 0x00,
    0x0f, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0xf0, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static const uint8_t arrow[60] PROGMEM = {
    0x00, 0x00, 0x00, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xf0, 0x00,
    0x0f, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x0f, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xf0, 0x00, 0x00
};

//TODO: Batteriestaende neu zeichnen
static const uint8_t batteryFull[60] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0f, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xf0
};

//TODO: Batteriestaende neu zeichnen
static const uint8_t batteryGood[60] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0x0f, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0x00, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xf0
};

//TODO: Batteriestaende neu zeichnen
static const uint8_t batteryLow[60] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0f, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xf0
};

//TODO: Batteriestaende neu zeichnen
static const uint8_t batteryEmpty[60] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0f, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xf0, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xf0, 0x00, 0xff, 0xff, 0xff,
    0x00, 0xff, 0xff, 0xff, 0xff, 0xf0
};

static const uint8_t clear[60] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t wait1[60] PROGMEM = {
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x00, 0xf0, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0xf0, 0x0f, 0xf0, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00,
    0x00, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0
};

static const uint8_t wait2[60] PROGMEM = {
    0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0,
    0x0f, 0xf0, 0x00, 0x00, 0x0f, 0xf0,
    0x0f, 0xff, 0x00, 0x00, 0xf0, 0xf0,
    0x0f, 0xff, 0xf0, 0x0f, 0xf0, 0xf0,
    0x0f, 0xff, 0xff, 0xff, 0x00, 0xf0,
    0x0f, 0xff, 0xff, 0xff, 0x00, 0xf0,
    0x0f, 0xff, 0xf0, 0x0f, 0xf0, 0xf0,
    0x0f, 0xff, 0x00, 0x00, 0xf0, 0xf0,
    0x0f, 0xf0, 0x00, 0x00, 0x0f, 0xf0,
    0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0
};

static const uint8_t wait3[60] PROGMEM = {
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x00, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x0f, 0xf0, 0x0f, 0xf0, 0x00,
    0x00, 0xf0, 0x00, 0x00, 0x0f, 0x00,
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0
};

static const uint8_t wait4[60] PROGMEM = {
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x00, 0xf0, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x0f, 0xf0, 0x0f, 0xf0, 0x00,
    0x00, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0
};

static const uint8_t good[60] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0xf0, 0x0f, 0xf0, 0x00,
    0x00, 0x0f, 0xf0, 0x0f, 0xf0, 0x00,
    0x00, 0x0f, 0xf0, 0x0f, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0,
    0x0f, 0xf0, 0x00, 0x00, 0x0f, 0xf0,
    0x00, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const uint8_t bad[60] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0xf0, 0x0f, 0xf0, 0x00,
    0x00, 0x0f, 0xf0, 0x0f, 0xf0, 0x00,
    0x00, 0x0f, 0xf0, 0x0f, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00,
    0x00, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x0f, 0xf0, 0x00, 0x00, 0x0f, 0xf0,
    0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};