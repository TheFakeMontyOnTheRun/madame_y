#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cpctelera.h>
#include "FixP.h"

#include "cpct_drw_mode0/cpct_drw_shared.h"
#include "cpct_drw_mode0/cpct_drw_mode0.h"

#define SCR_4000 (u8*)0x4000
#define SCR_C000 (u8*)0xC000

#define USE_FIXP_FOR_LINES

u8 page;

void shutdown() {
}


void clear() {
	cpct_memset_f64(SCR_C000, 0x0000, 16384);
}

void graphicsPut(int nColumn, int nLine, uint8_t nColor);

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

void fix_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int colour) {
#ifdef USE_FIXP_FOR_LINES
	if (x0 == x1) {

		int16_t _y0 = y0;
		int16_t _y1 = y1;

		if (y0 > y1) {
			_y0 = y1;
			_y1 = y0;
		}


		for (int16_t y = _y0; y <= _y1; ++y) {
			if (x0 < 0 || x0 >= 256 || y < 0 || y >= 128) {
				continue;
			}

			graphicsPut(x0, y, colour);
		}
		return;
	}

	if (y0 == y1) {
		int16_t _x0 = x0;
		int16_t _x1 = x1;

		if (x0 > x1) {
			_x0 = x1;
			_x1 = x0;
		}

		for (int16_t x = _x0; x <= _x1; ++x) {
			if (x < 0 || x >= 256 || y0 < 0 || y0 >= 128) {
				continue;
			}

			graphicsPut(x, y0, colour);
		}
		return;
	}

	//switching x0 with x1
	if (x0 > x1) {
		x0 = x0 + x1;
		x1 = x0 - x1;
		x0 = x0 - x1;

		y0 = y0 + y1;
		y1 = y0 - y1;
		y0 = y0 - y1;
	}

	{
		FixP_t fy = intToFix(y0);
		FixP_t fDeltatY = Div(intToFix(y1 - y0), intToFix(x1 - x0));

		for (int16_t x = x0; x <= x1; ++x) {
			int iy = fixToInt(fy);
			fy += fDeltatY;

			if (x < 0 || x >= 256 || iy < 0 || iy >= 128) {
				continue;
			}
			graphicsPut(x, iy, colour);

		}
	}
#else
	cpct_drw_setLineColour_mode0(colour);
	cpct_drw_line_mode0(x0 / 2, y0, x1 / 2, y1);
#endif
}

void graphicsFill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour) {


	unsigned char *pScreen = (unsigned char *)0xC000;
	int16_t nLine = 0;
	int16_t nColumn = 0;
	int16_t nPixel = 0;

	x0 = x0 / 2;
	x1 = x1 / 2;

	for(nColumn = x0; nColumn < x1; nColumn++)
	{
		if ( nColumn < 0 || nColumn >= 128 ) {
			continue;
		}

		nPixel = nColumn & 1;

		for(nLine = y0; nLine < y1; nLine++)
		{
			if ( nLine < 0 || nLine >= 200 ) {
				continue;
			}
			pScreen =  ((unsigned char *) 0xC000 + ((nLine / 8) * 80) + ((nLine % 8) * 2048)) + (nColumn >> 1);
			SetMode0PixelColor(pScreen, colour, nPixel);
		}
	}
}

void hLine(int16_t x0, int16_t x1, int16_t y, uint8_t colour) {
	fix_line( x0, y, x1, y, colour );
}

void vLine(int16_t x0, int16_t y0, int16_t y1, uint8_t colour) {
	unsigned char *pScreen = (unsigned char *) 0xC000;
	uint16_t base;
	unsigned char nByte;
	int16_t nLine = 0;
	int16_t nColumn = 0;
	int16_t nPixel = 0;
	uint8_t mask1 = 0;
	uint8_t mask2 = 0;

	x0 = x0 >> 1;

	nColumn = x0;

	if (nColumn < 0 || nColumn >= 128) {
		return;
	}

	nPixel = nColumn & 1;

	pScreen = (unsigned char *) 0xC000;

	if (nPixel == 0) {
		mask1 = 85;

		if (colour & 1)
			mask2 |= 128;

		if (colour & 2)
			mask2 |= 8;

		if (colour & 4)
			mask2 |= 32;

		if (colour & 8)
			mask2 |= 2;
	} else {
		mask1 = 170;

		if (colour & 1)
			mask2 |= 64;

		if (colour & 2)
			mask2 |= 4;

		if (colour & 4)
			mask2 |= 16;

		if (colour & 8)
			mask2 |= 1;
	}

	base = 0xC000 + (nColumn >> 1);

	if ( y0 > y1 ) {
		int16_t tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	for (nLine = y0; nLine < y1; nLine++) {
		if (nLine < 0 || nLine >= 128) {
			return;
		}
		pScreen =  ((unsigned char *) base + ((nLine / 8) * 80) + ((nLine % 8) * 2048)) ;
		nByte = *pScreen;
		nByte &= mask1;
		nByte |= mask2;
		*pScreen = nByte;
	}
}

void graphicsPut(int nColumn, int nLine, uint8_t nColor) {
#ifdef USE_FIXP_FOR_LINES
	unsigned char *pScreen = (unsigned char *) 0xC000;
	unsigned char nPixel = 0;
	unsigned char nByte;
	nColumn = nColumn >> 1;

	if (nColumn < 0 || nColumn >= 128 || nLine < 0 || nLine >= 200 ) {
		return;
	}

	nPixel = nColumn & 1;

	pScreen =  ((unsigned char *) 0xC000 + ((nLine / 8) * 80) + ((nLine % 8) * 2048)) + (nColumn >> 1);
	nByte = *pScreen;

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

	*pScreen = nByte;
#else
	fix_line(nColumn, nLine, nColumn, nLine, nColor);
#endif
}
