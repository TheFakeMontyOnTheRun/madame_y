#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cpctelera.h>

uint8_t frame = 0;

#define MOVING_POINTERS

void shutdownGraphics() {
}

void clearGraphics();


void graphicsPut(uint8_t nColumn, uint8_t nLine);

void writeStr(uint8_t nColumn, uint8_t nLine, char *str, uint8_t fg, uint8_t bg);

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

    cpct_memset_f64(0xC000, 0x0000, 16 * 1024);
    cpct_memset_f64(0x8000, 0x0000, 16 * 1024);

#ifndef MOVING_POINTERS
    frame = 1;
    cpct_setVideoMemoryPage(cpct_pageC0);
#else
    frame = 0;
    cpct_setVideoMemoryPage(cpct_page80);
#endif
}

void graphicsFlush() {
    cpct_waitVSYNC();
#ifdef MOVING_POINTERS
    if (frame) {
        cpct_setVideoMemoryPage(cpct_page80);
        frame = 0;

    } else {
        cpct_setVideoMemoryPage(cpct_pageC0);
        frame = 1;

    }
#else
    memcpy(0xC000, 0x8000, 80 * 200);
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

    uint8_t *pScreen = (uint8_t *) (frame) ? 0x8000 : 0xC000;
    unsigned char *pS;
    unsigned char *base;
    uint8_t nLine = y;
    uint8_t nColumn = 0;
    uint8_t rest;
    uint8_t bytes;
    uint8_t dx = (x1 - x0);
    bytes = dx >> 1;
    base = ((unsigned char *) pScreen + ((nLine & 248) * 10) + ((nLine & 7) << 11)) + (x0 >> 1);
//write whole bytes first, then the remainder with masks

    if (x0 & 1) {
        graphicsPut(x0, nLine);
        base++;
        dx--;
        bytes--;
        x0++;
    }

    memset(base, 0xFF, bytes);

    base += bytes;
    dx -= (bytes << 1);

    for (nColumn = 0; nColumn <= dx; nColumn++) {

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

    uint8_t *pScreen = (uint8_t *) (frame) ? 0x8000 : 0xC000;
    uint8_t *pointers[8];
//odd lines
//even lines
    uint8_t pointerIndex;
    unsigned char *pS;
    unsigned char *base;
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

    if (y0 > y1) {
        uint8_t tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    //pS = ((unsigned char *) base + ((nLine >> 3) * 80) + ((nLine & 7) << 11));
    pointers[0] = ((unsigned char *) base + (((0) & 248) * 10) + ((0) << 11));
    pointers[1] = pointers[0] + ((1) << 11);
    pointers[2] = pointers[0] + ((2) << 11);
    pointers[3] = pointers[0] + ((3) << 11);
    pointers[4] = pointers[0] + ((4) << 11);
    pointers[5] = pointers[0] + ((5) << 11);
    pointers[6] = pointers[0] + ((6) << 11);
    pointers[7] = pointers[0] + ((7) << 11);

    for (nLine = y0; nLine < y1; nLine++) {

        pointerIndex = nLine & 7;

        pS = pointers[pointerIndex ] + ((nLine & 248) * 10);

        nByte = *pS;
        nByte &= mask1;
        nByte |= mask2;
        *pS = nByte;
    }
}

void writeStr(uint8_t nColumn, uint8_t nLine, char *str, uint8_t fg, uint8_t bg) {
    unsigned char *pS = (uint8_t *) (frame) ? 0x8000 : 0xC000;
    unsigned char nPixel = 0;

    if (nColumn >= 128 || nLine >= 200) {
        return;
    }

    nPixel = nColumn & 1;

    pS = ((unsigned char *) pS + ((nLine & 248) * 10) + ((nLine & 7) << 11)) + (nColumn >> 1);

    cpct_drawStringM0(str, pS);
}

inline void graphicsPut(uint8_t nColumn, uint8_t nLine) {
    uint8_t *pScreen = (uint8_t *) (frame) ? 0x8000 : 0xC000;

    unsigned char *pS;
    unsigned char nByte = 0;

    pS = ((unsigned char *) pScreen + ((nLine & 248 ) * 10) + ((nLine & 7) << 11)) + (nColumn >> 1);
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

void clearGraphics() {
    memset((frame) ? 0x8000 : 0xC000, 0, 16 * 1024);
}