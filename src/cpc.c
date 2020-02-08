#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cpctelera.h>
#include "FixP.h"

#define SCR_4000 (u8*)0x4000
#define SCR_C000 (u8*)0xC000

//#define DBL_BUFFER

u8 page;
uint8_t* backBuffer;

void shutdown() {
}


void clear() {
#ifdef DBL_BUFFER
	cpct_memset_f64(backBuffer, 0, 80 * 200);
#else
	cpct_memset_f64((uint8_t*)0xC000, 0, 80 * 200);
#endif
}

void graphicsPut(uint8_t nColumn, uint8_t nLine, uint8_t nColor);

void writeStr( uint8_t nColumn, uint8_t nLine, char* str, uint8_t fg, uint8_t bg );

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

	cpct_setVideoMode(0);

	cpct_memset_f64(SCR_C000, 0x0000, 80 * 200);
#ifdef DBL_BUFFER
	backBuffer = (uint8_t*)malloc(80 * 200);
	clear();
#endif
	page = 0;
}

void graphicsFlush() {
#ifdef DBL_BUFFER
	cpct_memcpy(SCR_C000, backBuffer, 80 * 200);
#endif
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

void fix_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t colour) {

	if (x0 == x1) {

        uint8_t _y0 = y0;
        uint8_t _y1 = y1;

		if (y0 > y1) {
			_y0 = y1;
			_y1 = y0;
		}


		for (uint8_t y = _y0; y <= _y1; ++y) {
			if (y >= 128) {
				continue;
			}

			graphicsPut(x0, y, colour);
		}
		return;
	}

	if (y0 == y1) {
        uint8_t _x0 = x0;
        uint8_t _x1 = x1;

		if (x0 > x1) {
			_x0 = x1;
			_x1 = x0;
		}

		for (uint8_t x = _x0; x <= _x1; ++x) {
			if (y0 >= 128) {
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
		//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

		int8_t dx = abs(x1 - x0);
		int8_t sx = x0 < x1 ? 1 : -1;
		int8_t dy = -abs(y1 - y0);
		int8_t sy = y0 < y1 ? 1 : -1;
		int8_t err = dx + dy;  /* error value e_xy */
		int8_t e2;
		while (1) {
			graphicsPut(x0, y0, colour);
			/* loop */
			if (x0 == x1 && y0 == y1) return;
			e2 = 2 * err;

			if (e2 >= dy) {
				err += dy; /* e_xy+e_x > 0 */
				x0 += sx;
			}

			if (e2 <= dx) {
				/* e_xy+e_y < 0 */
				err += dx;
				y0 += sy;
			}
		}
	}
}

void graphicsFill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t colour) {
	uint8_t* pScreen = (uint8_t*)
#ifdef DBL_BUFFER
			backBuffer;
#else
			0xC000;
#endif

	unsigned char *pS;
    uint8_t nLine = 0;
    uint8_t nColumn = 0;
    uint8_t nPixel = 0;

	for(nColumn = x0; nColumn < x1; nColumn++)
	{
		if ( nColumn >= 128 ) {
			continue;
		}

		nPixel = nColumn & 1;

		for(nLine = y0; nLine < y1; nLine++)
		{
			if ( nLine >= 200 ) {
				continue;
			}
			pS =  ((unsigned char *) pScreen + ((nLine / 8) * 80) + ((nLine % 8) * 2048)) + (nColumn >> 1);
			SetMode0PixelColor(pS, colour, nPixel);
		}
	}
}

void hLine(uint8_t x0, uint8_t x1, uint8_t y, uint8_t nColor) {
	uint8_t* pScreen = (uint8_t*)
#ifdef DBL_BUFFER
			backBuffer;
#else
			0xC000;
#endif
	unsigned char *pS;
	unsigned char *base;
    uint8_t nLine = y;
    uint8_t nColumn = 0;
    uint8_t nPixel = 0;
	uint8_t mask1 = 0;
	uint8_t mask2 = 0;
	uint8_t mask3 = 0;
	uint8_t mask4 = 0;


	if (y >= 128) {
		return;
	}

	mask1 = 85;

	if (nColor & 1)
		mask2 |= 128;

	if (nColor & 2)
		mask2 |= 8;

	if (nColor & 4)
		mask2 |= 32;

	if (nColor & 8)
		mask2 |= 2;

	mask3 = 170;

	if (nColor & 1)
		mask4 |= 64;

	if (nColor & 2)
		mask4 |= 4;

	if (nColor & 4)
		mask4 |= 16;

	if (nColor & 8)
		mask4 |= 1;

	if (x0 >= 128) {
		x0 = 127;
	}

	if (x1 >= 128) {
		x1 = 127;
	}

	base = ((unsigned char *) pScreen + ((nLine / 8) * 80) + ((nLine % 8) * 2048));

	for (nColumn = x0; nColumn < x1; nColumn++) {
		unsigned char nByte;

		nPixel = nColumn & 1;
		pS = base + (nColumn >> 1);
		nByte = *pS;

		if (nPixel == 0) {
			nByte &= mask1;
			nByte |= mask2;

		} else {
			nByte &= mask3;
			nByte |= mask4;
		}

		*pS = nByte;
	}
}
void vLine(uint8_t x0, uint8_t y0, uint8_t y1, uint8_t colour) {
	uint8_t* pScreen = (uint8_t*)
#ifdef DBL_BUFFER
			backBuffer;
#else
			0xC000;
#endif

	unsigned char *pS;
	unsigned char * base;
	unsigned char nByte;
	uint8_t nLine = 0;
    uint8_t nColumn = 0;
    uint8_t nPixel = 0;
	uint8_t mask1 = 0;
	uint8_t mask2 = 0;

	nColumn = x0;

	if (nColumn >= 128) {
		return;
	}

	nPixel = nColumn & 1;

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

	base = pScreen + (nColumn >> 1);

	if ( y0 > y1 ) {
        uint8_t tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	if (y0 >= 128) {
		y0 = 127;
	}

	if (y1 >= 128) {
		y1 = 127;
	}

	for (nLine = y0; nLine < y1; nLine++) {
		pS =  ((unsigned char *) base + ((nLine / 8) * 80) + ((nLine % 8) * 2048)) ;
		nByte = *pS;
		nByte &= mask1;
		nByte |= mask2;
		*pS = nByte;
	}
}

void writeStr( uint8_t nColumn, uint8_t nLine, char* str, uint8_t fg, uint8_t bg ) {
	unsigned char *pS = (uint8_t*)
#ifdef DBL_BUFFER
            backBuffer;
#else
            0xC000;
#endif
	unsigned char nPixel = 0;

	if (nColumn >= 128 || nLine >= 200 ) {
		return;
	}

	nPixel = nColumn & 1;

	pS =  ((unsigned char *) pS + ((nLine / 8) * 80) + ((nLine % 8) * 2048)) + (nColumn >> 1);

	cpct_drawStringM0(str, pS, fg, bg );
}

void graphicsPut(uint8_t nColumn, uint8_t nLine, uint8_t nColor) {
	uint8_t* pScreen = (uint8_t*)
#ifdef DBL_BUFFER
			backBuffer;
#else
			0xC000;
#endif

	unsigned char *pS;
	unsigned char nPixel = 0;
	unsigned char nByte;

	if (nColumn >= 128 || nLine >= 128 ) {
		return;
	}

	nPixel = nColumn & 1;

	pS =  ((unsigned char *) pScreen + ((nLine / 8) * 80) + ((nLine % 8) * 2048)) + (nColumn >> 1);
	nByte = *pS;

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

	*pS = nByte;
}
