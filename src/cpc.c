#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cpctelera.h>

#include "cpct_drw_mode0/cpct_drw_shared.h"
#include "cpct_drw_mode0/cpct_drw_mode0.h"

#define SCR_4000 (u8*)0x4000
#define SCR_C000 (u8*)0xC000

u8 page;

void shutdown() {
}


void clear() {
	cpct_memset_f64(SCR_C000, 0x0000, 16384);


}

uint8_t getKey() {

	cpct_scanKeyboard_f();

	if (cpct_isKeyPressed(Key_W))
		return 'w';

	if (cpct_isKeyPressed(Key_S))
		return 's';

	if (cpct_isKeyPressed(Key_D))
		return 'd';

	if (cpct_isKeyPressed(Key_A))
		return 'a';


	return '.';
}

void init() {
	cpct_disableFirmware();


	cpct_drw_populateLineMasks_mode0();
	cpct_drw_setLineColour_mode0(1);

	cpct_memset_f64(SCR_C000, 0x0000, 16384);

	cpct_setVideoMemoryPage(cpct_pageC0);
	cpct_setVideoMode(0);

	page = 0x80;
	cpct_drw_setClippingBox_mode0(0, 128, 0, 128);
}

void graphicsFlush() {
	cpct_waitVSYNC();
}


void fix_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int colour) {
	cpct_drw_setLineColour_mode0(colour);
	cpct_drw_line_mode0(x0 / 2, y0, x1 / 2, y1);
}

unsigned char *GetLineAddress(unsigned char nLine) {
	return (unsigned char *) 0xC000 + ((nLine / 8) * 80) + ((nLine % 8) * 2048);
}

void SetMode0PixelColor(unsigned char *pByteAddress, unsigned char nColor, unsigned char nPixel) {
	unsigned char nByte = *pByteAddress;

	if (nPixel == 0) {
		nByte &= 85;

		if (nColor & 1)
			nByte |= 128;

		if (nColor & 2)
			nByte |= 8;

		if (nColor & 4)
			nByte |= 32;

		if (nColor & 8)
			nByte |= 2;
	} else {
		nByte &= 170;

		if (nColor & 1)
			nByte |= 64;

		if (nColor & 2)
			nByte |= 4;

		if (nColor & 4)
			nByte |= 16;

		if (nColor & 8)
			nByte |= 1;
	}

	*pByteAddress = nByte;
}

void graphicsFill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour) {

}


void graphicsPut(int nColumn, int nLine, uint8_t nColor) {

	unsigned char *pScreen = (unsigned char *) 0xC000;
	unsigned char nPixel = 0;

	nPixel = nColumn % 2;

	pScreen = GetLineAddress(nLine) + nColumn / 2;
	SetMode0PixelColor(pScreen, nColor, nPixel);
}
