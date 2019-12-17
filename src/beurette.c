#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "FixP.h"


void shutdown();

void graphicsPut(int x, int y, uint8_t colour);

void graphicsFill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour);

void clear();

uint8_t getKey();

void init();

void graphicsFlush();

void fix_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour);

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
		{3, -1}, //0
		{2, 0}, // 1
		{2, 1}, // 2
		{2, -1}, //3
		{3, -1} //4
};

const int8_t map[40][40] = {
		{0, 0, 0, 0, 4, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 4, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
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


	z1 = z0 + dZ;
	z0px = (projections[z0].px);
	z0py = (projections[z0].py);
	z1px = (projections[z1].px);
	z1py = (projections[z1].py);
	z0dx = (projections[z0].dx >> 3);
	z0dy = (projections[z0].dy >> 2);
	z1dx = (projections[z1].dx >> 3);
	z1dy = (projections[z1].dy >> 2);


	px0z0 = z0px - ((x0) * z0dx);
	py0z0 = z0py - ((y0) * z0dy);
	px1z0 = px0z0 - (dX * z0dx);
	py1z0 = py0z0 - (dY * z0dy);
	px0z1 = z1px - ((x0) * z1dx);
	py0z1 = z1py - ((y0) * z1dy);
	px1z1 = px0z1 - (dX * z1dx);
	py1z1 = py0z1 - (dY * z1dy);

/*
	drawQuad(px0z1, py0z1, px1z1, py1z1, 5);
	drawQuad(px0z0, py0z0, px1z0, py1z0, 5);
*/
	if (!low) {

		FixP_t heightDiff;
		FixP_t fy;
		FixP_t fDeltatY;
		int x, x0, x1;

		for (x = px0z0; x < px1z0; ++x) {
			if (stencilHigh[x] < py0z0) {
				fix_line(x, py0z0, x, stencilHigh[x], colour);
				stencilHigh[x] = py0z0;
			}
		}

		for (x = px0z1; x < px1z1; ++x) {
			if (stencilHigh[x] < py0z1) {
				fix_line(x, py0z1, x, stencilHigh[x], colour);
				stencilHigh[x] = py0z1;
			}
		}

		x0 = px0z0;
		x1 = px0z1;

		if ( x0 != x1 ) {

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
				if (stencilHigh[x] < iy) {
					fix_line(x, iy, x, stencilHigh[x], colour);
					stencilHigh[x] = iy;
				}
			}
		}

		x0 = px1z0;
		x1 = px1z1;

		if ( x0 != x1 ) {

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
				if (stencilHigh[x] < iy) {
					fix_line(x, iy, x, stencilHigh[x], colour);
					stencilHigh[x] = iy;
				}
			}
		}
	} else { /* LOW area */
		FixP_t heightDiff;
		FixP_t fy;
		FixP_t fDeltatY;
		int x, x0, x1;

		for (x = px0z0; x < px1z0; ++x) {
			if (stencilLow[x] > py1z0) {
				fix_line(x, py1z0, x, stencilLow[x], colour);
				stencilLow[x] = py1z0;
			}
		}

		for (x = px0z1; x < px1z1; ++x) {
			if (stencilLow[x] > py1z1) {
				fix_line(x, py1z1, x, stencilLow[x], colour);
				stencilLow[x] = py1z1;
			}
		}

		x0 = px0z0;
		x1 = px0z1;

		if ( x0 != x1 ) {

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
				if (stencilLow[x] > iy) {
					fix_line(x, iy, x, stencilLow[x], colour);
					stencilLow[x] = iy;
				}
			}
		}

		x0 = px1z0;
		x1 = px1z1;

		if ( x0 != x1 ) {

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
				if (stencilLow[x] > iy) {
					fix_line(x, iy, x, stencilLow[x], colour);
					stencilLow[x] = iy;
				}
			}
		}
	}
}

void drawPattern( int pattern, int x, int y, int cameraX, int cameraZ ) {

		int diff;

		diff = patterns[pattern].floor - patterns[0].floor;
		//if (diff)
		{
			drawCubeAt(x - cameraX, patterns[0].floor, cameraZ - y, 1,
					   diff, 1, 1, pattern );
		}

		diff = patterns[0].ceiling - patterns[pattern].ceiling;
		//if (diff)
		{
			drawCubeAt(x - cameraX, patterns[pattern].ceiling, cameraZ - y, 1,
					   diff, 1, 0, pattern );
		}

}

int main(int argc, char **argv) {
	int running = 1;
	int8_t x, y = 0;
	int8_t cameraX = 5;
	int8_t cameraZ = 5;

	init();

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
		memset(stencilLow, 0xFF, 256);
		memset(stencilHigh, 0, 256);


		for (int y = min(cameraZ, 40); y >= max(cameraZ - 40, 0); --y) {
			for (int x = cameraX; x < min(cameraX + 10, 40); ++x) {
				int pattern = map[y][x];
				if (pattern != 0) {
					drawPattern( pattern, x, y, cameraX, cameraZ);
					graphicsPut(x, y, pattern % 15);
				}
			}

			for ( x = max(cameraX - 1, 0); x >= max(cameraX - 10, 0) ; --x) {
				int pattern = map[y][x];
				if (pattern != 0) {
					drawPattern( pattern, x, y, cameraX, cameraZ);
					graphicsPut(x, y, pattern % 15);
				}
			}

		}

		graphicsPut(cameraX, cameraZ, 16);

		fix_line(0, 0, 255, 0, 2);
		fix_line(0, 128, 255, 128, 2);
		fix_line(255, 0, 255, 128, 2);
		fix_line(0, 0, 0, 128, 2);


		graphicsFlush();
	} while (running);

	shutdown();

	return 0;
}
