#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "FixP.h"

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

//void fix_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour);

void hLine(int16_t x0, int16_t x1, int16_t y, uint8_t colour);

void vLine(int16_t x0, int16_t y0, int16_t y1, uint8_t colour);

uint8_t stencilHigh[256];
uint8_t stencilLow[256];

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
				{0,   128, -1020, 512}, // 0
				{0,   128, -1020, 512}, // 1
				{64,  96,  -508,  256}, // 2
				{85,  85,  -340,  168}, // 3
				{96,  80,  -252,  128}, // 4
				{102, 77,  -204,  104}, // 5
				{106, 75,  -172,  88},  // 6
				{109, 73,  -148,  72},  // 7
				{112, 72,  -124,  64},  // 8
				{113, 71,  -116,  56},  // 9
				{115, 70,  -100,  48},  // 10
				{116, 70,  -92,   48},  // 11
				{117, 69,  -84,   40}, // 12
				{118, 69,  -76,   40}, // 13
				{118, 69,  -76,   40}, // 14
				{119, 68,  -68,   32}, // 15
				{120, 68,  -60,   32}, // 16
				{120, 68,  -60,   32}, // 17
				{120, 68,  -60,   32}, // 18
				{121, 67,  -52,   24}, // 19
				{121, 67,  -52,   24}, // 20
				{121, 67,  -52,   24}, // 21
				{122, 67,  -44,   24}, // 22
				{122, 67,  -44,   24}, // 23
				{122, 67,  -44,   24}, // 24
				{122, 67,  -44,   24}, // 25
				{123, 66,  -36,   16}, // 26
				{123, 66,  -36,   16}, // 27
				{123, 66,  -36,   16}, // 28
				{123, 66,  -36,   16}, // 29
				{123, 66,  -36,   16}, // 30
				{123, 66,  -36,   16}, // 31
				{124, 66,  -28,   16}, // 32
				{124, 66,  -28,   16}, // 33
				{124, 66,  -28,   16}, // 34
				{124, 66,  -28,   16}, // 35
				{124, 66,  -28,   16}, // 36
				{124, 66,  -28,   16}, // 37
				{124, 66,  -28,   16}, // 38
				{124, 66,  -28,   16}, // 39
				{124, 66,  -28,   16}, // 40
		};

const struct Pattern patterns[16] = {
		{5, -1, 0}, //0
		{2, 0,  0}, // 1
		{-1, -1,  1}, // 2
		{2, -1, 0}, //3
		{5, -1, 0}, //4
		{3, 2, 0} //5
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
#endif

	if (!low) {
		FixP_t heightDiff;
		FixP_t fy;
		FixP_t fDeltatY;
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

		if (py0z0 > py0z1) {
			for (x = px0z0; x < px1z0; ++x) {
				if (IN_RANGE(0, 255, x) && stencilHigh[x] < py0z0) {
#ifdef FILLED_POLYS
					fix_line(x, py0z0 + 1, x, stencilHigh[x] - 1, 5);
#endif
					if (drawContour) {
						graphicsPut(x, py0z0, 5);
					}
					stencilHigh[x] = py0z0;
				}
			}
		} else {
			for (x = px0z1; x < px1z1; ++x) {
				if (IN_RANGE(0, 255, x) && stencilHigh[x] < py0z1) {
#ifdef FILLED_POLYS
					fix_line(x, py0z1 + 1, x, stencilHigh[x] - 1, 5);
#endif
					if (drawContour) {
						graphicsPut(x, py0z1, 5);
					}
					stencilHigh[x] = py0z1;
				}
			}
		}
		x0 = px0z0;
		x1 = px0z1;

		if (x0 != x1) {

			if (x0 > x1) {
				x0 = x0 + x1;
				x1 = x0 - x1;
				x0 = x0 - x1;
				fy = intToFix(py0z1);
				heightDiff = (py0z0 - py0z1);
			} else {
				fy = intToFix(py0z0);
				heightDiff = (py0z1 - py0z0);
			}

			fDeltatY = Div(intToFix(heightDiff), intToFix(x1 - x0));

			for (x = x0; x < x1; ++x) {

				int iy = fixToInt(fy);
				fy += fDeltatY;
				if (IN_RANGE(0, 255, x) && stencilHigh[x] < iy) {
#ifdef FILLED_POLYS
					fix_line(x, iy + 1, x, stencilHigh[x] - 1, 5);
#endif
					if (drawContour) {
						graphicsPut(x, iy, 5);
					}
					stencilHigh[x] = iy;
				}
			}
		}

		x0 = px1z0;
		x1 = px1z1;

		if (x0 != x1) {

			if (x0 > x1) {
				x0 = x0 + x1;
				x1 = x0 - x1;
				x0 = x0 - x1;
				fy = intToFix(py0z1);
				heightDiff = (py0z0 - py0z1);
			} else {
				fy = intToFix(py0z0);
				heightDiff = (py0z1 - py0z0);
			}

			fDeltatY = Div(intToFix(heightDiff), intToFix(x1 - x0));

			for (x = x0; x < x1; ++x) {

				int iy = fixToInt(fy);
				fy += fDeltatY;
				if (IN_RANGE(0, 255, x) && stencilHigh[x] < iy) {
#ifdef FILLED_POLYS
					fix_line(x, iy + 1, x, stencilHigh[x] - 1, 5);
#endif
					if (drawContour) {
						graphicsPut(x, iy, 5);
					}
					stencilHigh[x] = iy;
				}
			}
		}


	} else { /* ------------ LOW area ------------ */



		FixP_t heightDiff;
		FixP_t fy;
		FixP_t fDeltatY;
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
						fix_line(x, py1z1 + 1, x, stencilLow[x] - 1, 5);
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
						fix_line(x, py1z0 + 1, x, stencilLow[x] - 1, 5);
#endif
						graphicsPut(x, py1z0, 6);
					}
					stencilLow[x] = py1z0;
				}
			}
		}

		x0 = px0z0;
		x1 = px0z1;

		if (x0 != x1) {

			if (x0 > x1) {
				x0 = x0 + x1;
				x1 = x0 - x1;
				x0 = x0 - x1;
				fy = intToFix(py1z1);
				heightDiff = (py1z0 - py1z1);
			} else {
				fy = intToFix(py1z0);
				heightDiff = (py1z1 - py1z0);
			}

			fDeltatY = Div(intToFix(heightDiff), intToFix(x1 - x0));

			for (x = x0; x < x1; ++x) {

				int iy = fixToInt(fy);
				fy += fDeltatY;
				if (IN_RANGE(0, 255, x) && stencilLow[x] > iy) {
					if (drawContour) {
#ifdef FILLED_POLYS
						fix_line(x, iy + 1, x, stencilLow[x] - 1, 5);
#endif

						graphicsPut(x, iy, 5);
					}
					stencilLow[x] = iy;
				}
			}
		}

		x0 = px1z0;
		x1 = px1z1;

		if (x0 != x1) {

			if (x0 > x1) {
				x0 = x0 + x1;
				x1 = x0 - x1;
				x0 = x0 - x1;
				fy = intToFix(py1z1);
				heightDiff = (py1z0 - py1z1);
			} else {
				fy = intToFix(py1z0);
				heightDiff = (py1z1 - py1z0);
			}

			fDeltatY = Div(intToFix(heightDiff), intToFix(x1 - x0));

			for (x = x0; x < x1; ++x) {

				int iy = fixToInt(fy);
				fy += fDeltatY;
				if (IN_RANGE(0, 255, x) && stencilLow[x] > iy) {
					if (drawContour) {
#ifdef FILLED_POLYS
						fix_line(x, iy + 1, x, stencilLow[x] - 1, 5);
#endif
						graphicsPut(x, iy, 5);
					}

					stencilLow[x] = iy;
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
	int8_t cameraX = 7;
	int8_t cameraZ = 15;
	int lastPattern, lastIndex;

	init();
	memset(stencilLow, 0xFF, 256);
	memset(stencilHigh, 0, 256);


	do {


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
#ifndef SDLSW
			default:
			  goto waitkey;
#endif
		}

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

		clear();

		vLine(255, 0, 127, 2);
		vLine(0, 0, 127, 2);

		hLine(0, 255, 0, 2);
		hLine(0, 255, 127, 2);



		for (int y = min(cameraZ - 3, 40); y >= max(cameraZ - 40, 0); --y) {
			int x;
			lastIndex = cameraX;
			lastPattern = map[y][lastIndex];

			for (x = lastIndex; x < min(cameraX + 4 + ((cameraZ - 3) - y), 40); ++x) {
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

			for (x = lastIndex; x >= max(cameraX - 2 - ((cameraZ - 3) - y), 0); --x) {
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
		memset(stencilLow, 0xFF, 256);
		memset(stencilHigh, 0, 256);

	} while (running);

	shutdown();

	return 0;
}
