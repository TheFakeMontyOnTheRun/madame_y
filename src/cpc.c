#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cpctelera.h>
#include "FixP.h"

#define SCR_C000 (u8*)0xC000

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

void graphicsPut(uint8_t nColumn, uint8_t nLine);

void writeStr( uint8_t nColumn, uint8_t nLine, char* str, uint8_t fg, uint8_t bg );

uint8_t getKey() {

	cpct_scanKeyboard_f();
    if (cpct_isKeyPressed(Key_Q))
        return 'q';

    if (cpct_isKeyPressed(Key_E))
        return 'e';

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
}

void graphicsFlush() {
#ifdef DBL_BUFFER
	cpct_memcpy(SCR_C000, backBuffer, 80 * 200);
#endif
}

void fix_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {

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

			graphicsPut(x0, y);
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

			graphicsPut(x, y0);
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
			graphicsPut(x0, y0);
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

void hLine(uint8_t x0, uint8_t x1, uint8_t y) {
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
    uint8_t rest;
    uint8_t bytes;
	base = ((unsigned char *) pScreen + ((nLine >> 3) * 80) + ((nLine & 7) << 11));
//write whole bytes first, then the remainder with masks

    bytes = ( x1 - x0 ) >> 1;

    memset( base, 0xFF, bytes);

	for (nColumn = x0 + ( bytes << 1 ); nColumn < x1; nColumn++) {
		unsigned char nByte;

		pS = base + (nColumn >> 1);
		nByte = *pS;

		if (nColumn & 1) {
            nByte &= 170;
            nByte |= 64;
		} else {
            nByte &= 85;
            nByte |= 128;
		}

		*pS = nByte;
	}
}
void vLine(uint8_t x0, uint8_t y0, uint8_t y1) {
	uint8_t* pScreen = (uint8_t*)
#ifdef DBL_BUFFER
			backBuffer;
#else
			0xC000;
#endif
//odd lines
//even lines
	unsigned char *pS;
	unsigned char * base;
	unsigned char nByte;
	uint8_t nLine = 0;
	uint8_t mask1 = 0;
	uint8_t mask2 = 0;

	if (x0 & 1) {
        mask1 = 170;
        mask2 = 64;
    } else {
        mask1 = 85;
        mask2 = 128;
    }

	base = pScreen + (x0 >> 1);

	if ( y0 > y1 ) {
        uint8_t tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	for (nLine = y0; nLine < y1; nLine++) {
		pS =  ((unsigned char *) base + ((nLine >> 3) * 80) + ((nLine & 7) << 11)) ;
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

	pS =  ((unsigned char *) pS + ((nLine >> 3) * 80) + ((nLine & 7) << 11)) + (nColumn >> 1);

	cpct_drawStringM0(str, pS );
}

void graphicsPut(uint8_t nColumn, uint8_t nLine) {
	uint8_t* pScreen = (uint8_t*)
#ifdef DBL_BUFFER
			backBuffer;
#else
			0xC000;
#endif

	unsigned char *pS;
	unsigned char nByte = 0;

	pS =  ((unsigned char *) pScreen + ((nLine >> 3 ) * 80) + ((nLine & 7) << 11)) + (nColumn >> 1);
	nByte = *pS;

	if (nColumn & 1) {
        nByte &= 170;
        nByte |= 64;
    } else {
        nByte &= 85;
        nByte |= 128;
    }

	*pS = nByte;
}
