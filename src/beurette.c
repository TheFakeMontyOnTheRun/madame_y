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
};


struct Pattern {
    int8_t ceiling : 4;
    uint8_t elementsMask: 4;
};

const struct Projection projections[16] =
        {
                //                                   Z
                {0,  127, -64}, // 0
                {0,  127, -64}, // 1
                {20, 106, -43}, // 2
                {31, 95,  -32}, // 3
                {37, 89,  -26}, // 4
                {42, 84,  -21}, // 5
                {45, 81,  -18},  // 6
                {47, 79,  -16},  // 7
                {49, 77,  -14},  // 8
                {50, 76,  -13},  // 9
                {51, 75,  -12},  // 10
                {52, 74,  -11},  // 11
                {53, 73,  -10}, // 12
                {54, 72,  -9}, // 13
                {54, 72,  -9}, // 14
                {55, 71,  -8}, // 15
        };

const struct Pattern patterns[10] = {
        {4,  3}, //0
        {2,  3}, // 1
        {-1, 0}, // 2
        {-1, 3}, //3
        {0,  3}, //4
        {1,  3}, //5
        {2,  3}, //6
        {4,  3}, //7
        {-1, 1}, // 8
        {-1, 2}, // 9
};

const int8_t map[32][32] = {
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 6, 7, 7, 7, 3, 3, 3, 3, 7, 8, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 3, 6, 7, 7, 3, 4, 4, 4, 7, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 8, 7, 7, 7, 7, 7, 7, 8, 2, 2, 4, 4, 6, 7, 7, 3, 4, 5, 5, 7, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 7, 7, 6, 6, 6, 7, 2, 2, 8, 7, 5, 6, 7, 7, 3, 4, 5, 6, 7, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 7, 7, 6, 3, 6, 7, 2, 2, 2, 7, 7, 7, 7, 7, 3, 4, 5, 7, 7, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 9, 7, 7, 6, 6, 6, 7, 2, 2, 9, 7, 7, 7, 7, 7, 3, 4, 7, 7, 7, 9, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 8, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 2, 8, 7, 7, 3, 7, 7, 7, 7, 7, 7, 7, 8, 2, 2, 2, 7},
        {2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 9, 2, 2, 2, 2, 9, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 8, 7, 7, 7, 7, 7, 9, 2, 2, 2, 7},
        {2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 7, 7, 8, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 7, 7, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 9, 7, 7, 7, 7, 8, 2, 2, 9, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 7, 7, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 7, 7, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 7, 7, 9, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 7, 7, 7, 7, 2, 2, 2, 2, 8, 7, 7, 7, 7, 3, 2, 2, 2, 2, 7, 7, 7, 4, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 7, 7, 7, 7, 9, 2, 2, 2, 9, 7, 7, 7, 7, 7, 3, 2, 2, 9, 7, 7, 7, 8, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 9, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7}
};

int8_t max(int8_t x1, int8_t x2) {
    return x1 > x2 ? x1 : x2;
}

int8_t min(int8_t x1, int8_t x2) {
    return x1 < x2 ? x1 : x2;
}

void drawCubeAt(int8_t x0, int8_t y0, int8_t z0, int8_t dX, int8_t dY, int8_t dZ, uint8_t elementMask, uint8_t colour) {

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

    if (z0 >= 16) {
        z0 = 15;
    }

    z1 = z0 + dZ;

    if (z1 >= 16) {
        z1 = 15;
    }


    z0px = (projections[z0].px);
    z1px = (projections[z1].px);
    z0dx = ((projections[z0].dx));
    z1dx = ((projections[z1].dx));

    px0z0 = z0px - ((x0) * z0dx);
    px0z1 = z1px - ((x0) * z1dx);

    if ((px0z0 > 127) && (px0z1 > 127)) {
        return;
    }

    px1z0 = px0z0 - (dX * z0dx);
    px1z1 = px0z1 - (dX * z1dx);

    if ((px1z0 < 0) && (px1z1 < 0)) {
        return;
    }

    z1py = (projections[z1].py);
    z0dy = -z0dx;
    z1dy = -z1dx;
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

    fix_line( px0z0, py0z0, px0z1, py0z1, 4);
    fix_line( px1z0, py0z0, px1z1, py0z1, 4);
    return;
#endif

    {
        int16_t x, x0, x1;

        if (drawContour) {
            if (elementMask & 2) {
                if (IN_RANGE(0, 127, px0z0) && stencilHigh[px0z0] < py0z0) {
                    vLine(px0z0, py0z0, stencilHigh[px0z0] + 1, colour);
                }

                if (IN_RANGE(0, 127, px1z0) && stencilHigh[px1z0] < py0z0) {
                    vLine(px1z0, py0z0, stencilHigh[px1z0] + 1, colour);
                }
            }

            if ( elementMask & 1 ) {
                if (IN_RANGE(0, 127, px0z1) && px0z1 < px0z0 && py0z1 > stencilHigh[px0z1]) {
                    vLine(px0z1, py0z1, stencilHigh[px0z1] + 1, colour);
                }

                if (IN_RANGE(0, 127, px1z1) && px1z1 > px1z0 && py0z1 > stencilHigh[px1z1]) {
                    vLine(px1z1, py0z1, stencilHigh[px1z1] + 1, colour);
                }
            }
        }

        /* Draw the horizontal outlines of z0 and z1 */

        if (py0z0 > py0z1) {
            /* Ceiling is lower than camera */
            for (x = px0z0; x <= px1z0; ++x) {
                if (IN_RANGE(0, 127, x) && stencilHigh[x] < py0z0) {
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
            for (x = px0z0; x <= px1z0; ++x) {
                if (IN_RANGE(0, 127, x) && stencilHigh[x] < py0z0) {
                    graphicsPut(x, py0z0, colour);
                    graphicsPut(x, stencilHigh[x], colour);
                }
            }
        }


        /* The left segment */
        x0 = px0z0;
        x1 = px0z1;

        if (x0 != x1) {
            int8_t y0 = py0z0;
            int8_t y1 = py0z1;
            int8_t dx = abs(x1 - x0);
            int8_t sx = x0 < x1 ? 1 : -1;
            int8_t dy = -abs(y1 - y0);
            int8_t sy = y0 < y1 ? 1 : -1;
            int8_t err = dx + dy;  /* error value e_xy */
            int8_t e2;

            while ((x0 != x1 || y0 != y1)) {

                if (IN_RANGE(0, 127, x0)) {
                    if (stencilHigh[x0] < y0) {
#ifdef FILLED_POLYS
                        vLine(x0, y0 + 1, stencilHigh[x0] - 1, 5);
#endif
                        if (drawContour) {
                            graphicsPut(x0, y0, colour);
                            graphicsPut(x0, stencilHigh[x0], colour);
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


        /* The right segment */
        x0 = px1z0;
        x1 = px1z1;

        if (x0 != x1) {
            int8_t y0 = py0z0;
            int8_t y1 = py0z1;
            int8_t dx = abs(x1 - x0);
            int8_t sx = x0 < x1 ? 1 : -1;
            int8_t dy = -abs(y1 - y0);
            int8_t sy = y0 < y1 ? 1 : -1;
            int8_t err = dx + dy;  /* error value e_xy */
            int8_t e2;
            while ((x0 != x1 || y0 != y1)) {

                if (IN_RANGE(0, 127, x0) && stencilHigh[x0] < y0) {
#ifdef FILLED_POLYS
                    vLine(x0, y0 + 1, stencilHigh[x0] - 1, 5);
#endif
                    if (drawContour) {
                        graphicsPut(x0, y0, colour);
                        graphicsPut(x0, stencilHigh[x0], colour);
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


        /* Draw the horizontal outlines of z0 and z1 */

        if (py0z0 <= py0z1) {
            /* Ceiling is higher than the camera*/
            /* Draw the last segment */

            for (x = px0z1; x <= px1z1; ++x) {
                if (IN_RANGE(0, 127, x) && stencilHigh[x] < py0z1) {
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
               diff, 1, patterns[pattern].elementsMask, pattern);
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

        vLine(127, 0, 127, 5);
        vLine(0, 0, 127, 5);
        hLine(0, 127, 0, 5);
        hLine(0, 127, 127, 5);
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
