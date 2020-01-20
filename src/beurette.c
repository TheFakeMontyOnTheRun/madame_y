#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#ifndef MSX
//#define FILLED_POLYS
#endif

#define IN_RANGE(V0, V1, V)  ((V0) <= (V) && (V) <= (V1))

void shutdown();

void graphicsPut(int x, int y, uint8_t colour);

void graphicsFill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour);

void clear();

uint8_t getKey();

void init();

void graphicsFlush();

void fix_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int colour);

void hLine(int16_t x0, int16_t x1, int16_t y, uint8_t colour);

void vLine(int16_t x0, int16_t y0, int16_t y1, uint8_t colour);

uint8_t stencilHigh[64];
uint8_t stencilLow[64];

int8_t cameraX = 33;
int8_t cameraY = 2;
int8_t cameraZ = 22;

struct Projection {
	uint8_t px;
	uint8_t py;
	int16_t dx;
	int16_t dy;
};


struct Pattern {
	int16_t ceiling;
	int16_t floor;
	uint8_t blockVisibility;
};

const struct Projection projections[41] =
		{
				//                                   Z
				{0,   128, -255, 512}, // 0
				{0,   128, -255, 512}, // 1
				{16,  96,  -127,  256}, // 2
				{21,  85,  -85,  168}, // 3
				{24,  80,  -63,  128}, // 4
				{25, 77,  -51,  104}, // 5
				{26, 75,  -43,  88},  // 6
				{27, 73,  -37,  72},  // 7
				{28, 72,  -31,  64},  // 8
				{28, 71,  -29,  56},  // 9
				{28, 70,  -25,  48},  // 10
				{29, 70,  -23,   48},  // 11
				{29, 69,  -21,   40}, // 12
				{29, 69,  -19,   40}, // 13
				{29, 69,  -19,   40}, // 14
				{29, 68,  -17,   32}, // 15
				{30, 68,  -15,   32}, // 16
				{30, 68,  -15,   32}, // 17
				{30, 68,  -15,   32}, // 18
				{30, 67,  -13,   24}, // 19
				{30, 67,  -13,   24}, // 20
				{30, 67,  -13,   24}, // 21
				{30, 67,  -11,   24}, // 22
				{30, 67,  -11,   24}, // 23
				{30, 67,  -11,   24}, // 24
				{30, 67,  -11,   24}, // 25
				{30, 66,  -9,   16}, // 26
				{30, 66,  -9,   16}, // 27
				{31, 66,  -9,   16}, // 28
				{31, 66,  -9,   16}, // 29
				{31, 66,  -9,   16}, // 30
				{31, 66,  -9,   16}, // 31
				{31, 66,  -7,   16}, // 32
				{31, 66,  -7,   16}, // 33
				{31, 66,  -7,   16}, // 34
				{31, 66,  -7,   16}, // 35
				{31, 66,  -7,   16}, // 36
				{31, 66,  -7,   16}, // 37
				{31, 66,  -7,   16}, // 38
				{32, 66,  -7,   16}, // 39
				{32, 66,  -7,   16}, // 40
		};

const struct Pattern patterns[16] = {
		{5, -1, 0}, //0
		{2, 0,  0}, // 1
		{-1, -1,  1}, // 2
		{2, -1, 0}, //3
		{5, -1, 0}, //4
		{3, 2, 0}, //5
		{5, 1, 0} //6
};

const int8_t map[40][40] = {
		{4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 2, 4, 4, 4, 4, 4, 4, 4, 2, 4, 2, 2, 2, 2, 2, 4, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 2, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 4, 4, 4, 2, 4, 2, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 2, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 2, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 2, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 2, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 2, 2, 3, 3, 3, 2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 6, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}
};

int max(int x1, int x2) {
	return x1 > x2 ? x1 : x2;
}

int min(int x1, int x2) {
	return x1 < x2 ? x1 : x2;
}

void drawCubeAt(int8_t x0, int8_t y0, int8_t z0, int8_t dX, int8_t dY, int8_t dZ, int low, int colour) {

	int8_t z1;
	uint8_t z0px;
	uint8_t z0py;
	uint8_t z1px;
	uint8_t z1py;
	int16_t z0dx;
	int16_t z0dy;
	int16_t z1dx;
	int16_t z1dy;

	int16_t px0z0;
	int16_t py0z0;
	int16_t px1z0;
	int16_t py1z0;
	int16_t px0z1;
	int16_t py0z1;
	int16_t px1z1;
	int16_t py1z1;

	int drawContour;

	z1 = z0 + dZ;

	z0px = (projections[z0].px);
	z1px = (projections[z1].px);
	z0dx = (projections[z0].dx >> 3);
	z1dx = (projections[z1].dx >> 3);

	px0z0 = z0px - ((x0) * z0dx);
	px0z1 = z1px - ((x0) * z1dx);

	if ((px0z0 > 255 ) && (px0z1 > 255 )) {
		return;
	}

	px1z0 = px0z0 - (dX * z0dx);
	px1z1 = px0z1 - (dX * z1dx);

	if ((px1z0 < 0 ) && (px1z1 < 0 )) {
		return;
	}

	z1py = (projections[z1].py);
	z0dy = (projections[z0].dy >> 2);
	z1dy = (projections[z1].dy >> 2);
	z0py = (projections[z0].py);

	py0z0 = z0py - ((y0) * z0dy);
	py1z0 = py0z0 - (dY * z0dy);
	py0z1 = z1py - ((y0) * z1dy);
	py1z1 = py0z1 - (dY * z1dy);

	drawContour = (py0z0 != py1z0 );

#ifdef DEBUG_WIREFRAME
	fix_line( px0z0, py0z0, px1z0, py0z0, 4);
	fix_line( px0z0, py0z0, px0z0, py1z0, 4);
	fix_line( px1z0, py0z0, px1z0, py1z0, 4);
	fix_line( px0z0, py1z0, px1z0, py1z0, 4);

	fix_line( px0z1, py0z1, px1z1, py0z1, 4);
	fix_line( px0z1, py0z1, px0z1, py1z1, 4);
	fix_line( px1z1, py0z1, px1z1, py1z1, 4);
	fix_line( px0z1, py1z1, px1z1, py1z1, 4);

	fix_line( px0z0, py0z0, px0z1, py0z1, 4);
	fix_line( px1z0, py0z0, px1z1, py0z1, 4);
	fix_line( px0z0, py1z0, px0z1, py1z1, 4);
	fix_line( px1z0, py1z0, px1z1, py1z1, 4);
	return;
#endif

	if (!low) {
		int x, x0, x1;

		if (drawContour ) {
			if (IN_RANGE(0, 255, px0z0) && stencilHigh[px0z0] < py0z0 ) {
				vLine(px0z0, py0z0, stencilHigh[px0z0] + 1, 5);
			}

			if (IN_RANGE(0, 255, px1z0) && stencilHigh[px1z0] < py0z0) {
				vLine(px1z0, py0z0, stencilHigh[px1z0] + 1, 5);
			}

			if (IN_RANGE(0, 255, px0z1) && px0z1 < px0z0 && py0z1 > stencilHigh[px0z1]) {
				vLine(px0z1, py0z1, stencilHigh[px0z1] + 1, 5);
			}

			if (IN_RANGE(0, 255, px1z1) && px1z1 > px1z0 && py0z1 > stencilHigh[px1z1]) {
				vLine(px1z1, py0z1, stencilHigh[px1z1] + 1, 5);
			}
		}

		/* Draw the horizontal outlines of z0 and z1 */

		if (py0z0 > py0z1) {
		    /* Ceiling is lower than camera */
			for (x = px0z0; x < px1z0; ++x) {
				if (IN_RANGE(0, 255, x) && stencilHigh[x] < py0z0) {
#ifdef FILLED_POLYS
					vLine(x, py0z0 + 1, stencilHigh[x] - 1, 5);
#endif
					if (drawContour) {
						graphicsPut(x, py0z0, 5);
					}
					stencilHigh[x] = py0z0;
				}
			}
		} else {
            /* Ceiling is higher than the camera*/
            /* Let's just draw the nearer segment */
            for (x = px0z0; x < px1z0; ++x) {
                if (IN_RANGE(0, 255, x) && stencilHigh[x] < py0z0) {
                    if (drawContour) {
                        graphicsPut(x, py0z0, 5);
                    }
                }
            }
		}


		/* The left segment */
		x0 = px0z0;
		x1 = px0z1;

		if (x0 != x1) {
			{
				int y0 = py0z0;
				int y1 = py0z1;
				int dx = abs(x1 - x0);
				int sx = x0 < x1 ? 1 : -1;
				int dy = -abs(y1 - y0);
				int sy = y0 < y1 ? 1 : -1;
				int err = dx + dy;  /* error value e_xy */
				int e2;

				while ((x0 != x1 || y0 != y1)) {

                    if (IN_RANGE(0, 255, x0)  ) {
                        if (py0z0 < py0z1) {
                            if (drawContour) {
                                graphicsPut(x0, stencilHigh[x0], 5);
                            }
                        }

                        if (stencilHigh[x0] < y0) {
#ifdef FILLED_POLYS
                            vLine(x0, y0 + 1, stencilHigh[x0] - 1, 5);
#endif
                            if (drawContour) {
                                graphicsPut(x0, y0, 5);
                            }
                            stencilHigh[x0] = y0;
                        }
                    }



					/* loop */
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


		/* The right segment */
		x0 = px1z0;
		x1 = px1z1;

		if (x0 != x1) {

			{
				int y0 = py0z0;
				int y1 = py0z1;
				int dx = abs(x1 - x0);
				int sx = x0 < x1 ? 1 : -1;
				int dy = -abs(y1 - y0);
				int sy = y0 < y1 ? 1 : -1;
				int err = dx + dy;  /* error value e_xy */
				int e2;
				while ((x0 != x1 || y0 != y1)) {

					if (IN_RANGE(0, 255, x0) && stencilHigh[x0] < y0) {
#ifdef FILLED_POLYS
						vLine(x0, y0 + 1, stencilHigh[x0] - 1, 5);
#endif
						if (drawContour) {
							graphicsPut(x0, y0, 5);
						}
						stencilHigh[x0] = y0;
					}

					/* loop */
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


        /* Draw the horizontal outlines of z0 and z1 */

        if (py0z0 < py0z1) {
            /* Ceiling is higher than the camera*/
            /* Draw the last segment */

            for (x = px0z1; x <= px1z1; ++x) {
                if (IN_RANGE(0, 255, x) && stencilHigh[x] < py0z1) {
#ifdef FILLED_POLYS
                    vLine(x, py0z1 + 1, stencilHigh[x] - 1, 5);
#endif
                    if (drawContour) {
                        graphicsPut(x, py0z1, 5);
                    }
                    stencilHigh[x] = py0z1;
                }
            }
        }



	} else { /* ------------ LOW area ------------ */

		int x, x0, x1;

		if (drawContour ) {
			if (IN_RANGE(0, 255, px0z0) && stencilLow[px0z0] > py1z0) {
				vLine(px0z0, py1z0, stencilLow[px0z0] - 1, 5);
			}

			if (IN_RANGE(0, 255, px1z0) && stencilLow[px1z0] > py1z0) {
				vLine(px1z0, py1z0, stencilLow[px1z0] - 1, 5);
			}

			if (IN_RANGE(0, 255, px0z1) && px0z1 < px0z0 && py1z1 < stencilLow[px0z1]) {
				vLine(px0z1, py1z1, stencilLow[px0z1] - 1, 5);
			}

			if (IN_RANGE(0, 255, px1z1) && px1z1 > px1z0 && py1z1 < stencilLow[px1z1]) {
				vLine(px1z1, py1z1, stencilLow[px1z1] - 1, 5);
			}
		}

		if (py1z0 > py1z1) {
			for (x = px0z1; x < px1z1; ++x) {
				if (IN_RANGE(0, 255, x) && stencilLow[x] > py1z1) {
					if (drawContour) {
#ifdef FILLED_POLYS
						vLine(x, py1z1 + 1, stencilLow[x] - 1, 5);
#endif
						graphicsPut(x, py1z1, 5);
					}

					stencilLow[x] = py1z1;
				}
			}
		} else {
			for (x = px0z0; x < px1z0; ++x) {
				if (IN_RANGE(0, 255, x) && stencilLow[x] > py1z0) {
					if (drawContour) {
#ifdef FILLED_POLYS
						vLine(x, py1z0 + 1, stencilLow[x] - 1, 5);
#endif
						graphicsPut(x, py1z0, 5);
					}
					stencilLow[x] = py1z0;
				}
			}
		}

		x0 = px0z0;
		x1 = px0z1;

		if (x0 != x1) {


			{
				int y0 = py1z0;
				int y1 = py1z1;
				int dx = abs(x1 - x0);
				int sx = x0 < x1 ? 1 : -1;
				int dy = -abs(y1 - y0);
				int sy = y0 < y1 ? 1 : -1;
				int err = dx + dy;  /* error value e_xy */
				int e2;
				while ((x0 != x1 || y0 != y1)) {

					if (IN_RANGE(0, 255, x0) && stencilLow[x0] > y0) {
#ifdef FILLED_POLYS
						vLine(x0, y0 + 1, stencilLow[x0] - 1, 5);
#endif
						if (drawContour) {
							graphicsPut(x0, y0, 5);
						}
						stencilLow[x0] = y0;
					}


					/* loop */
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

		x0 = px1z0;
		x1 = px1z1;

		if (x0 != x1) {

			{
				int y0 = py1z0;
				int y1 = py1z1;
				int dx = abs(x1 - x0);
				int sx = x0 < x1 ? 1 : -1;
				int dy = -abs(y1 - y0);
				int sy = y0 < y1 ? 1 : -1;
				int err = dx + dy;  /* error value e_xy */
				int e2;
				while ((x0 != x1 || y0 != y1)) {

					if (IN_RANGE(0, 255, x0) && stencilLow[x0] > y0) {
#ifdef FILLED_POLYS
						vLine(x0, y0 + 1, stencilLow[x0] - 1, 5);
#endif
						if (drawContour) {
							graphicsPut(x0, y0, 5);
						}
						stencilLow[x0] = y0;
					}

					/* loop */
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
	}
}

void drawPattern(int pattern, int x0, int x1, int y, int cameraX, int cameraZ) {

	int diff;

	diff = patterns[pattern].floor - patterns[0].floor;
	//if (diff)
	{
		drawCubeAt(x0 - cameraX, patterns[0].floor, cameraZ - y, x1 - x0,
				   diff, 1, 1, pattern);
	}

	diff = patterns[0].ceiling - patterns[pattern].ceiling;
	//if (diff)

	{
		drawCubeAt(x0 - cameraX, patterns[pattern].ceiling, cameraZ - y, x1 - x0,
				   diff, 1, 0, pattern);
	}
}

int main(int argc, char **argv) {
	int running = 1;
	int8_t x, y = 0;
	int lastPattern, lastIndex;
	int prevX;
	int prevZ;
	cameraX = 5;
	cameraY = 0;
	cameraZ = 18;


	init();
	memset(stencilLow, 0xFF, 64);
	memset(stencilHigh, 0, 64);


	do {
		prevX = cameraX;
		prevZ = cameraZ;

		if (cameraZ >= 40) {
			cameraZ = 39;
		}

		if (cameraX >= 40) {
			cameraX = 39;
		}

		if (cameraZ < 0) {
			cameraZ = 0;
		}

		if (cameraX < 0) {
			cameraX = 0;
		}

		if ( patterns[map[cameraZ - 2][cameraX]].floor  != patterns[0].floor || patterns[map[cameraZ - 2][cameraX]].ceiling < 1 ) {
			cameraX = prevX;
			cameraZ = prevZ;
		}

		clear();

		vLine(64, 0, 127, 5);
		vLine(0, 0, 127, 5);
		hLine(0, 64, 0, 5);
		hLine(0, 64, 127, 5);



		for (int y = min(cameraZ - 3, 40); y >= max(cameraZ - 40, 0); --y) {
			int x;
			lastIndex = cameraX;
			lastPattern = map[y][lastIndex];

			for (x = lastIndex; x < min(cameraX + 5 + ((cameraZ - 3) - y), 40); ++x) {
				int pattern = map[y][x];

				if (pattern != lastPattern ) {
					if (lastPattern != 0) {
						drawPattern(lastPattern, lastIndex, x, y, cameraX, cameraZ);
						lastIndex = x;
						lastPattern = pattern;
					}
				}
			}
			if (lastPattern != 0) {
				drawPattern(lastPattern, lastIndex, x, y, cameraX, cameraZ);
			}

			lastIndex = max(cameraX - 1, 0);
			lastPattern = map[y][lastIndex];

			for (x = lastIndex; x >= max(cameraX - 3 - ((cameraZ - 3) - y), 0); --x) {
				int pattern = map[y][x];

				if (pattern != lastPattern ) {
					if (lastPattern != 0) {
						drawPattern(lastPattern, x + 1, lastIndex + 1, y, cameraX, cameraZ);
						lastIndex = x;
						lastPattern = pattern;
					}
				}
			}
			if (lastPattern != 0) {
				drawPattern(lastPattern, x + 1, lastIndex + 1, y, cameraX, cameraZ);
			}

		}

		graphicsPut(cameraX, cameraZ, 16);


		graphicsFlush();

		memset(stencilLow, 0xFF, 64);
		memset(stencilHigh, 0, 64);

		waitkey:
		switch (getKey()) {
			case 'a':
				cameraX--;
				break;
			case 'd':
				cameraX++;
				break;
			case 's':
				cameraZ++;
				break;
			case 'q':
				running = 0;
				break;
			case 'w':
				cameraZ--;
				break;
#if !defined(SDLSW) || !defined(AMIGA)
			default:
			  goto waitkey;
#endif
		}
	} while (running);

	shutdown();

	return 0;
}
