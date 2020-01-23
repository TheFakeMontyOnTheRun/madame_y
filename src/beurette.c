#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#ifndef MSX
//#define FILLED_POLYS
#endif

#define IN_RANGE(V0, V1, V)  ((V0) <= (V) && (V) <= (V1))

void shutdown();

void writeStr(uint8_t nColumn, uint8_t nLine, char *str, uint8_t fg, uint8_t bg);

void graphicsPut(uint8_t x, uint8_t y, uint8_t colour);

void graphicsFill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t colour);

void clear();

uint8_t getKey();

void init();

void graphicsFlush();

void fix_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t colour);

void hLine(uint8_t x0, uint8_t x1, uint8_t y, uint8_t colour);

void vLine(uint8_t x0, uint8_t y0, uint8_t y1, uint8_t colour);

int8_t stencilHigh[128];

int8_t cameraX = 33;
int8_t cameraZ = 22;

struct Projection {
    uint8_t px;
    uint8_t py;
    int16_t dx;
    int16_t dy;
};


struct Pattern {
    int8_t ceiling;
    int8_t floor;
    int8_t elementsMask;
};

const struct Projection projections[32] =
        {
                //                                   Z
                {0,  128, -255, 512}, // 0
                {0,  128, -255, 512}, // 1
                {16, 96,  -127, 256}, // 2
                {21, 85,  -85,  168}, // 3
                {24, 80,  -63,  128}, // 4
                {25, 77,  -51,  104}, // 5
                {26, 75,  -43,  88},  // 6
                {27, 73,  -37,  72},  // 7
                {28, 72,  -31,  64},  // 8
                {28, 71,  -29,  56},  // 9
                {28, 70,  -25,  48},  // 10
                {29, 70,  -23,  48},  // 11
                {29, 69,  -21,  40}, // 12
                {29, 69,  -19,  40}, // 13
                {29, 69,  -19,  40}, // 14
                {29, 68,  -17,  32}, // 15
                {30, 68,  -15,  32}, // 16
                {30, 68,  -15,  32}, // 17
                {30, 68,  -15,  32}, // 18
                {30, 67,  -13,  24}, // 19
                {30, 67,  -13,  24}, // 20
                {30, 67,  -13,  24}, // 21
                {30, 67,  -11,  24}, // 22
                {30, 67,  -11,  24}, // 23
                {30, 67,  -11,  24}, // 24
                {30, 67,  -11,  24}, // 25
                {30, 66,  -9,   16}, // 26
        };

const struct Pattern patterns[16] = {
        {3,  -1}, //0
        {2,  0}, // 1
        {-1, -1}, // 2
        {-1,  -1}, //3
        {0,  -1}, //4
        {1,  2}, //5
        {2,  0}, //6
        {3,  -1} //7
};

const int8_t map[32][32] = {
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 2, 7, 7, 7, 7, 5, 6, 2, 7, 7, 3, 3, 3, 3, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 2, 7, 7, 7, 7, 4, 3, 6, 2, 7, 3, 4, 4, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 2, 2, 2, 7, 7, 7, 7, 4, 4, 6, 2, 7, 3, 4, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 2, 7, 6, 6, 6, 7, 7, 7, 5, 6, 2, 7, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 2, 2, 7, 6, 2, 6, 7, 7, 7, 7, 7, 2, 7, 3, 4, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 2, 7, 7, 6, 6, 6, 7, 7, 7, 7, 7, 2, 7, 3, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 7, 3, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
        {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7}
};

int8_t max(int8_t x1, int8_t x2) {
    return x1 > x2 ? x1 : x2;
}

int8_t min(int8_t x1, int8_t x2) {
    return x1 < x2 ? x1 : x2;
}

void drawCubeAt(int8_t x0, int8_t y0, int8_t z0, int8_t dX, int8_t dY, int8_t dZ, uint8_t low, uint8_t colour) {

    int8_t z1;
    uint8_t z0px;
    uint8_t z0py;
    uint8_t z1px;
    uint8_t z1py;
    int8_t z0dx;
    int8_t z0dy;
    int8_t z1dx;
    int8_t z1dy;

    int16_t px0z0;
    int8_t py0z0;
    int16_t px1z0;
    int8_t py1z0;
    int16_t px0z1;
    int8_t py0z1;
    int16_t px1z1;

    uint8_t drawContour;

    if (colour == 3) {
        colour = 3;
    }

    if (z0 >= 26) {
        z0 = 26;
    }

    z1 = z0 + dZ;

    if (z1 >= 26) {
        z1 = 26;
    }


    z0px = (projections[z0].px * 2);
    z1px = (projections[z1].px * 2);
    z0dx = ((projections[z0].dx * 2) >> 3);
    z1dx = ((projections[z1].dx * 2) >> 3);

    px0z0 = z0px - ((x0) * z0dx);
    px0z1 = z1px - ((x0) * z1dx);

    if ((px0z0 > 255) && (px0z1 > 255)) {
        return;
    }

    px1z0 = px0z0 - (dX * z0dx);
    px1z1 = px0z1 - (dX * z1dx);

    if ((px1z0 < 0) && (px1z1 < 0)) {
        return;
    }

    z1py = (projections[z1].py);
    z0dy = (projections[z0].dy >> 2);
    z1dy = (projections[z1].dy >> 2);
    z0py = (projections[z0].py);

    py0z0 = z0py - ((y0) * z0dy);
    py1z0 = py0z0 - (dY * z0dy);
    py0z1 = z1py - ((y0) * z1dy);


    drawContour = (py0z0 != py1z0);

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

    {
        int16_t x, x0, x1;

        if (drawContour) {
            if (IN_RANGE(0, 255, px0z0) && stencilHigh[px0z0] < py0z0) {
                vLine(px0z0, py0z0, stencilHigh[px0z0] + 1, colour);
            }

            if (IN_RANGE(0, 255, px1z0) && stencilHigh[px1z0] < py0z0) {
                vLine(px1z0, py0z0, stencilHigh[px1z0] + 1, colour);
            }

            if (IN_RANGE(0, 255, px0z1) && px0z1 < px0z0 && py0z1 > stencilHigh[px0z1]) {
                vLine(px0z1, py0z1, stencilHigh[px0z1] + 1, colour);
            }

            if (IN_RANGE(0, 255, px1z1) && px1z1 > px1z0 && py0z1 > stencilHigh[px1z1]) {
                vLine(px1z1, py0z1, stencilHigh[px1z1] + 1, colour);
            }
        }

        /* Draw the horizontal outlines of z0 and z1 */

        if (py0z0 > py0z1) {
            /* Ceiling is lower than camera */
            for (x = px0z0; x < px1z0; ++x) {
                if (IN_RANGE(0, 255, x) && stencilHigh[x] < py0z0) {
#ifdef FILLED_POLYS
                    vLine(x, py0z0 + 1, stencilHigh[x] - 1, colour);
#endif
                    if (drawContour) {
                        graphicsPut(x, py0z0, colour);
                        graphicsPut(x, stencilHigh[x], colour);
                    }
                    stencilHigh[x] = py0z0;
                }
            }
        } else if (drawContour) {
            /* Ceiling is higher than the camera*/
            /* Let's just draw the nearer segment */
            for (x = px0z0; x < px1z0; ++x) {
                if (IN_RANGE(0, 255, x) && stencilHigh[x] < py0z0) {
                    graphicsPut(x, py0z0, colour);
                    graphicsPut(x, stencilHigh[x], colour);
                }
            }
        }


        /* The left segment */
        x0 = px0z0;
        x1 = px0z1;

        if (x0 != x1) {
            {
                int8_t y0 = py0z0;
                int8_t y1 = py0z1;
                int8_t dx = abs(x1 - x0);
                int8_t sx = x0 < x1 ? 1 : -1;
                int8_t dy = -abs(y1 - y0);
                int8_t sy = y0 < y1 ? 1 : -1;
                int8_t err = dx + dy;  /* error value e_xy */
                int8_t e2;

                while ((x0 != x1 || y0 != y1)) {

                    if (IN_RANGE(0, 255, x0)) {
                        if (drawContour && py0z0 < py0z1) {
                            graphicsPut(x0, stencilHigh[x0], colour);
                        }

                        if (stencilHigh[x0] < y0) {
#ifdef FILLED_POLYS
                            vLine(x0, y0 + 1, stencilHigh[x0] - 1, 5);
#endif
                            if (drawContour) {
                                graphicsPut(x0, y0, colour);
                                graphicsPut(x0, stencilHigh[x0] + 1, colour);
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
                int8_t y0 = py0z0;
                int8_t y1 = py0z1;
                int8_t dx = abs(x1 - x0);
                int8_t sx = x0 < x1 ? 1 : -1;
                int8_t dy = -abs(y1 - y0);
                int8_t sy = y0 < y1 ? 1 : -1;
                int8_t err = dx + dy;  /* error value e_xy */
                int8_t e2;
                while ((x0 != x1 || y0 != y1)) {

                    if (IN_RANGE(0, 255, x0) && stencilHigh[x0] < y0) {
#ifdef FILLED_POLYS
                        vLine(x0, y0 + 1, stencilHigh[x0] - 1, 5);
#endif
                        if (drawContour) {
                            graphicsPut(x0, y0, colour);
                            graphicsPut(x0, stencilHigh[x0] + 1, colour);
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
                    vLine(x, py0z1 + 1, stencilHigh[x] - 1, colour);
#endif
                    if (drawContour) {
                        graphicsPut(x, py0z1, colour);
                    }
                    stencilHigh[x] = py0z1;
                }
            }
        }
    }
}

void drawPattern(uint8_t pattern, uint8_t x0, uint8_t x1, uint8_t y, uint8_t cameraX, uint8_t cameraZ) {

    int8_t diff;
    diff = patterns[0].ceiling - patterns[pattern].ceiling;
    drawCubeAt(x0 - cameraX, patterns[pattern].ceiling, cameraZ - y, x1 - x0,
               diff, 1, 0, pattern);
}

int main(int argc, char **argv) {
    uint8_t running = 1;
    int8_t x, y = 0;
    uint8_t lastPattern, lastIndex;
    uint8_t prevX;
    uint8_t prevZ;
    cameraX = 5;
    cameraZ = 18;

    init();

    memset(stencilHigh, 0, 128);


    do {
        clear();

        vLine(128, 0, 127, 5);
        vLine(0, 0, 127, 5);
        hLine(0, 128, 0, 5);
        hLine(0, 128, 127, 5);


        for (int8_t y = min(cameraZ - 3, 31); y >= max(cameraZ - 19, 0); --y) {
            int8_t x;
            lastIndex = cameraX;
            lastPattern = map[y][lastIndex];

            for (x = lastIndex; x < min(cameraX + 5 + ((cameraZ - 3) - y), 31); ++x) {
                uint8_t pattern = map[y][x];

                if (pattern != lastPattern) {
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
                uint8_t pattern = map[y][x];

                if (pattern != lastPattern) {
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

        memset(stencilHigh, 0, 128);

        prevX = cameraX;
        prevZ = cameraZ;

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

        if (cameraZ >= 32) {
            cameraZ = 31;
        }

        if (cameraX >= 32) {
            cameraX = 31;
        }

        if (cameraZ < 0) {
            cameraZ = 0;
        }

        if (cameraX < 0) {
            cameraX = 0;
        }

        if (patterns[map[cameraZ - 2][cameraX]].ceiling < 2) {
            cameraX = prevX;
            cameraZ = prevZ;
        }



    } while (running);

    shutdown();

    return 0;
}
