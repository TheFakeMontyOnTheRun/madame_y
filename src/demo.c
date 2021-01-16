#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef CPC_PLATFORM
#include <cpctelera.h>

uint8_t getFrame();

extern const uint16_t lineStart[127];
#endif


#define WEDGE_TYPE_NEAR_LEFT 4
#define WEDGE_TYPE_NEAR_RIGHT 8

enum DIRECTION {
    DIRECTION_N,
    DIRECTION_E,
    DIRECTION_S,
    DIRECTION_W
};

#define IN_RANGE(V0, V1, V)  ((V0) <= (V) && (V) <= (V1))


void shutdownGraphics();

void clearGraphics();

void writeStr(uint8_t nColumn, uint8_t nLine, char *str, uint8_t fg, uint8_t bg);

void graphicsPut(uint8_t x, uint8_t y);

uint8_t getKey();

void init();

void graphicsFlush();

void fix_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

void hLine(uint8_t x0, uint8_t x1, uint8_t y);

void vLine(uint8_t x0, uint8_t y0, uint8_t y1);

int8_t stencilHigh[128];

int8_t cameraX = 33;
int8_t cameraZ = 22;
int8_t cameraRotation = 0;
uint8_t running = 1;

struct Projection {
    uint8_t px;
    uint8_t py;
    int16_t dx;
};


struct Pattern {
    int8_t ceiling: 4;
    uint8_t elementsMask: 4;
    uint8_t geometryType;
    uint8_t block;
};

const struct Projection projections[36] =
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
            {54, 71,  -9}, // 14
            {55, 70,  -8}, // 15
            {55, 69,  -8}, // 16
            {56, 68,  -8}, // 17
            {56, 67,  -7}, // 18
            {57, 66,  -7}, // 19
            {57, 65,  -6}, // 20
            {57, 64,  -6}, // 21
            {57, 63,  -6},  // 22
            {58, 62,  -6},  // 23
            {58, 61,  -5},  // 24
            {58, 60,  -5},  // 25
            {58, 59,  -5},  // 26
            {58, 58,  -5},  // 27
            {59, 57,  -5}, // 28
            {59, 56,  -4}, // 29
            {59, 55,  -4}, // 30
            {59, 54,  -4}, // 31
            {59, 53,  -3}, // 32
            {59, 52,  -3}, // 33
            {59, 51,  -3}, // 34
            {59, 50,  -3}, // 35
        };

const struct Pattern patterns[16] = {
        {5,  3, 0, 0}, //0
        {5,  3, 0, 1}, // 1
        {-1, 0, 0, 0}, // 2
        {-1, 3, 0, 0}, //3
        {-1, 3, 4, 0}, //4
        {-1, 3, 8, 0}, //5
        {2,  3, 0, 0}, //6
        {5,  3, 0, 0}, //7
        {-1, 1, 0, 0}, // 8
        {-1, 3, 0, 0}, // 9
};

const int8_t map[32][32] = {
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 2, 8, 7, 7, 7, 7, 7, 8, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 2, 2, 7, 0, 0, 0, 7, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 2, 7, 7, 7, 7, 7, 9, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 2, 7, 7, 8, 8, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 2, 7, 7, 2, 2, 1, 1, 1, 1, 2, 7, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 1, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 2, 7, 7, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 8, 3, 3, 3, 7, 8, 2, 1, 0, 0, 0},
        {1, 2, 2, 2, 2, 2, 2, 7, 7, 2, 9, 2, 8, 2, 2, 2, 2, 2, 2, 1, 1, 2, 7, 7, 7, 7, 2, 2, 1, 0, 0, 0},
        {1, 2, 8, 7, 7, 5, 3, 6, 6, 3, 4, 7, 7, 7, 7, 7, 7, 5, 8, 1, 1, 2, 7, 7, 7, 7, 2, 1, 1, 0, 0, 0},
        {1, 2, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 7, 7, 2, 1, 1, 2, 7, 7, 6, 7, 2, 1, 1, 0, 0, 0},
        {1, 2, 9, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 1, 2, 7, 7, 7, 7, 2, 2, 1, 1, 1, 0},
        {1, 2, 2, 8, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 1, 2, 7, 7, 0, 7, 9, 2, 2, 2, 1, 0},
        {1, 1, 2, 3, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 1, 9, 7, 7, 0, 7, 7, 7, 8, 2, 1, 0},
        {0, 1, 2, 3, 6, 7, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 7, 2, 1, 1, 7, 7, 7, 0, 0, 0, 7, 2, 2, 1, 0},
        {0, 1, 2, 3, 6, 7, 0, 0, 0, 0, 7, 3, 7, 0, 0, 0, 0, 7, 2, 1, 1, 0, 0, 7, 0, 0, 0, 7, 2, 1, 1, 0},
        {0, 1, 2, 2, 6, 7, 0, 0, 0, 0, 7, 7, 7, 0, 0, 0, 0, 7, 2, 1, 7, 7, 7, 7, 0, 0, 0, 7, 2, 2, 1, 0},
        {0, 1, 1, 2, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 2, 2, 8, 7, 0, 7, 7, 7, 9, 2, 1, 0},
        {0, 0, 1, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 2, 2, 2, 7, 0, 7, 8, 2, 2, 2, 1, 0},
        {0, 0, 1, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 2, 2, 2, 7, 0, 7, 2, 2, 1, 1, 1, 0},
        {0, 0, 1, 2, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 1, 2, 2, 2, 7, 0, 7, 2, 2, 1, 0, 0, 0},
        {0, 0, 1, 2, 7, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 9, 1, 2, 2, 9, 7, 7, 7, 9, 2, 1, 0, 0, 0},
        {0, 0, 1, 2, 7, 0, 0, 7, 8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 7, 7, 7, 8, 2, 2, 1, 0, 0, 0},
        {0, 0, 1, 2, 7, 0, 0, 7, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 7, 0, 7, 2, 2, 1, 1, 0, 0, 0},
        {0, 1, 1, 2, 7, 0, 0, 7, 2, 2, 2, 2, 9, 7, 0, 0, 7, 7, 3, 1, 2, 9, 7, 0, 7, 2, 1, 1, 0, 0, 0, 0},
        {0, 1, 2, 2, 7, 0, 0, 7, 2, 7, 7, 7, 7, 7, 0, 0, 0, 7, 7, 7, 7, 7, 7, 0, 7, 2, 2, 1, 0, 0, 0, 0},
        {0, 1, 2, 9, 7, 7, 7, 7, 2, 7, 7, 7, 7, 7, 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 9, 2, 1, 0, 0, 0, 0},
        {0, 1, 2, 2, 2, 2, 2, 2, 9, 2, 2, 2, 8, 7, 0, 0, 0, 7, 8, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 7, 0, 0, 7, 7, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 7, 7, 7, 7, 8, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int8_t max(int8_t x1, int8_t x2) {
    return x1 > x2 ? x1 : x2;
}

int8_t min(int8_t x1, int8_t x2) {
    return x1 < x2 ? x1 : x2;
}


uint8_t drawWedge(int8_t x0, int8_t y0, int8_t z0, int8_t dX, int8_t dY, int8_t dZ, uint8_t elementMask, uint8_t type) {

    int16_t z1;
    int16_t z0px;
    int16_t z1px;
    int16_t z0py;

    int16_t z1py;
    int16_t z0dx;
    int16_t z1dx;

    int16_t px0z0;
    int16_t py0z0;
    int16_t py0z1;

    int16_t py1z0;
    int16_t py1z1;

    int16_t px1z1;

    if (z0 >= 32) {
        return 0;
    }

    z1 = z0 + dZ;

    if (z0 <= 2) {
        return 0;
    }

    if (z1 <= 2) {
        return 0;
    }

    if (z1 >= 32) {
        return 0;
    }


    if (type == WEDGE_TYPE_NEAR_LEFT) {
        z0px = (projections[z0].px);
        z1px = (projections[z1].px);
        z0dx = ((projections[z0].dx));
        z1dx = ((projections[z1].dx));

        px0z0 = z0px - (x0 * z0dx);
        px1z1 = z1px - ((dX + x0) * z1dx);

        z1py = (projections[z1].py);
        z0py = (projections[z0].py);

        py0z0 = z0py + ((y0) * z0dx);
        py0z1 = z1py + ((y0) * z1dx);

        py1z0 = z0py + ((y0 + dY) * z0dx);
        py1z1 = z1py + ((y0 + dY) * z1dx);

    } else {
        z0px = (projections[z1].px);
        z1px = (projections[z0].px);
        z0dx = ((projections[z1].dx));
        z1dx = ((projections[z0].dx));

        px0z0 = z0px - ((x0) * z0dx);
        px1z1 = z1px - ((x0 + dX) * z1dx); //extra operations to avoid overflow

        z1py = (projections[z0].py);
        z0py = (projections[z1].py);

        py0z0 = z0py + ((y0) * z0dx);
        py0z1 = z1py + ((y0) * z1dx);

        py1z0 = z0py + ((y0 + dY) * z0dx);
        py1z1 = z1py + ((y0 + dY) * z1dx);
    }

    if (px1z1 < 0 || px0z0 > 127) {
        return 0;
    }

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
        int16_t x0, x1;


        if (elementMask & 2) {
            if (IN_RANGE(0, 127, px0z0) && stencilHigh[px0z0] < py0z0) {
                vLine(px0z0, py0z0, max(py1z0, stencilHigh[px0z0]));
            }
        }

        if (elementMask & 1) {
            if (IN_RANGE(0, 127, px1z1) && py0z1 > stencilHigh[px1z1]) {
                vLine(px1z1, py0z1, max(py1z1, stencilHigh[px1z1]));
            }
        }
        

        /* The upper segment */
        x0 = px0z0;
        x1 = px1z1;

        if (x0 != x1) {
            int16_t upperY0 = py1z0;
            int16_t upperY1 = py1z1;
            int16_t upperDx = abs(x1 - x0);
            int16_t upperSx = x0 < x1 ? 1 : -1;
            int16_t upperDy = -abs(upperY1 - upperY0);
            int16_t upperSy = upperY0 < upperY1 ? 1 : -1;
            int16_t upperErr = upperDx + upperDy;  /* error value e_xy */
            int16_t upperErr2;
            int16_t lowerY0 = py0z0;
            int16_t lowerY1 = py0z1;
            int16_t lowerDx = abs(x1 - x0);
            int16_t lowerSx = x0 < x1 ? 1 : -1;
            int16_t lowerDy = -abs(lowerY1 - lowerY0);
            int16_t lowerSy = lowerY0 < lowerY1 ? 1 : -1;
            int16_t lowerErr = lowerDx + lowerDy;  /* error value e_xy */
            int16_t lowerErr2;
            
            while ((x0 != x1 && (upperY0 != upperY1 || lowerY0 != lowerY1))) {
                
                if (IN_RANGE(0, 127, x0)) {
                    if (stencilHigh[x0] <= upperY0) {
                        graphicsPut(x0, upperY0);
                    }
                    
                    if (stencilHigh[x0] < lowerY0) {
                        stencilHigh[x0] = lowerY0;
                    }
                }
                
                /* loop */
                upperErr2 = upperErr * 2;
                
                if (upperErr2 >= upperDy || lowerErr2 >= lowerDy) {
                    upperErr += upperDy; /* e_xy+e_x > 0 */
                    lowerErr += lowerDy; /* e_xy+e_x > 0 */
                    x0 += lowerSx;
                }
                
                if (x0 >= 128) {
                    return 0;
                }
                
                if (upperErr2 <= upperDx) {
                    /* e_xy+e_y < 0 */
                    upperErr += upperDx;
                    upperY0 += upperSy;
                }
                
                /* loop */
                lowerErr2 = lowerErr * 2;
                
                if (lowerErr2 <= lowerDx) {
                    /* e_xy+e_y < 0 */
                    lowerErr += lowerDx;
                    lowerY0 += lowerSy;
                }
            }
        }
    }

    return 1;
}

uint8_t drawCubeAt(int8_t x0, int8_t y0, int8_t z0, int8_t dX, int8_t dY, int8_t dZ, uint8_t elementMask) {

    int8_t z1;
    uint8_t z0px;
    uint8_t z0py;
    uint8_t z1px;
    uint8_t z1py;
    int8_t z0dx;
    int8_t z1dx;

    int16_t px0z0;
    int8_t py0z0;
    int16_t px1z0;
    int8_t py1z0;
    int16_t px0z1;
    int8_t py0z1;
    int16_t px1z1;

    uint8_t drawContour;

    if (z0 >= 32) {
        return 0;
    }

    z1 = z0 + dZ;

    if (z1 >= 32) {
        return 0;
    }


    z0px = (projections[z0].px);
    z1px = (projections[z1].px);
    z0dx = ((projections[z0].dx));
    z1dx = ((projections[z1].dx));

    px0z0 = z0px - ((x0) * z0dx);
    px0z1 = z1px - ((x0) * z1dx);

    px1z0 = px0z0 - (dX * z0dx);
    px1z1 = px0z1 - (dX * z1dx);

    z1py = (projections[z1].py);
    z0py = (projections[z0].py);

    py0z0 = z0py + ((y0) * z0dx);
    py1z0 = py0z0 + (dY * z0dx);
    py0z1 = z1py + ((y0) * z1dx);

    if (px1z0 < 0 || px0z0 > 127) {
        return 0;
    }

    drawContour = (dY);

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
                    vLine(px0z0, py0z0, stencilHigh[px0z0]);
                }

                if (IN_RANGE(0, 127, px1z0) && stencilHigh[px1z0] < py0z0) {
                    vLine(px1z0, py0z0, stencilHigh[px1z0]);
                }
            }

            if (elementMask & 1) {
                if (IN_RANGE(0, 127, px0z1) && px0z1 < px0z0 && py0z1 > stencilHigh[px0z1]) {
                    vLine(px0z1, py0z1, stencilHigh[px0z1]);
                }

                if (IN_RANGE(0, 127, px1z1) && px1z1 > px1z0 && py0z1 > stencilHigh[px1z1]) {
                    vLine(px1z1, py0z1, stencilHigh[px1z1]);
                }
            }
        }

        /* Draw the horizontal outlines of z0 and z1 */

        if (py0z0 > py0z1) {
            /* Ceiling is lower than camera */
            for (x = px0z0; x <= px1z0; ++x) {
                if (IN_RANGE(0, 127, x) && stencilHigh[x] < py0z0) {
                    if (drawContour) {
                        graphicsPut(x, stencilHigh[x]);
                    }
                    stencilHigh[x] = py0z0;
                }
            }
        } else if (drawContour) {
            /* Ceiling is higher than the camera*/
            /* Let's just draw the nearer segment */
            for (x = px0z0; x <= px1z0; ++x) {
                if (IN_RANGE(0, 127, x) && stencilHigh[x] < py0z0) {
                    graphicsPut(x, stencilHigh[x]);
                }
            }
        }


        /* The left segment */
        x0 = px0z0;
        x1 = px0z1;

        if (x0 != x1) {
            int16_t y0 = py0z0;
            int16_t y1 = py0z1;
            int16_t dx = abs(x1 - x0);
            int16_t sx = x0 < x1 ? 1 : -1;
            int16_t dy = -abs(y1 - y0);
            int16_t sy = y0 < y1 ? 1 : -1;
            int16_t err = dx + dy;  /* error value e_xy */
            int16_t e2;

            while ((x0 != x1 || y0 != y1)) {

                if (IN_RANGE(0, 127, x0)) {
                    if (stencilHigh[x0] < y0) {
                        if (drawContour) {
                            graphicsPut(x0, stencilHigh[x0]);
                        }
                        stencilHigh[x0] = y0;
                    }
                }

                /* loop */
                e2 = err << 2;

                if (e2 >= dy) {
                    err += dy; /* e_xy+e_x > 0 */
                    x0 += sx;
                }

                if (x0 >= 128) {
                    goto right_stroke;
                }

                if (e2 <= dx) {
                    /* e_xy+e_y < 0 */
                    err += dx;
                    y0 += sy;
                }
            }
        }

        right_stroke:

        /* The right segment */
        x0 = px1z0;
        x1 = px1z1;

        if (x0 != x1) {
            int16_t y0 = py0z0;
            int16_t y1 = py0z1;
            int16_t dx = abs(x1 - x0);
            int16_t sx = x0 < x1 ? 1 : -1;
            int16_t dy = -abs(y1 - y0);
            int16_t sy = y0 < y1 ? 1 : -1;
            int16_t err = dx + dy;  /* error value e_xy */
            int16_t e2;

            while ((x0 != x1 || y0 != y1)) {

                if (IN_RANGE(0, 127, x0) && stencilHigh[x0] < y0) {
                    if (drawContour) {
                        graphicsPut(x0, stencilHigh[x0]);
                    }
                    stencilHigh[x0] = y0;
                }

                /* loop */
                e2 = err << 2;

                if (e2 >= dy) {
                    err += dy; /* e_xy+e_x > 0 */
                    x0 += sx;
                }

                if (x0 >= 128) {
                    goto final_stroke;
                }

                if (e2 <= dx) {
                    /* e_xy+e_y < 0 */
                    err += dx;
                    y0 += sy;
                }
            }
        }

        final_stroke:
        if (py0z0 <= py0z1) {
            /* Ceiling is higher than the camera*/
            /* Draw the last segment */

            if (drawContour) {
                for (x = px0z1; x <= px1z1; ++x) {
                    if (IN_RANGE(0, 127, x) && stencilHigh[x] < py0z1) {
                        stencilHigh[x] = py0z1;
                    }
                }
            } else {
                for (x = px0z1; x <= px1z1; ++x) {
                    if (IN_RANGE(0, 127, x) && stencilHigh[x] < py0z1) {
                        stencilHigh[x] = py0z1;
                    }
                }
            }
        }
    }

    return 1;
}


uint8_t drawPattern(uint8_t pattern, uint8_t x0, uint8_t x1, uint8_t y) {

    int8_t diff = patterns[0].ceiling - patterns[pattern].ceiling;
    uint8_t type = patterns[pattern].geometryType;

    if (patterns[pattern].block) {
        return 0;
    }

    if (type == 0) {
        return drawCubeAt(x0, patterns[pattern].ceiling, y, x1 - x0,
                          diff, 1, patterns[pattern].elementsMask);

    } else {
        switch (cameraRotation) {
            case DIRECTION_W:
            case DIRECTION_E:
                if (type == WEDGE_TYPE_NEAR_LEFT) {
                    type = WEDGE_TYPE_NEAR_RIGHT;
                } else {
                    type = WEDGE_TYPE_NEAR_LEFT;
                }
                break;

        }

        return drawWedge(x0, patterns[pattern].ceiling, y, x1 - x0,
                         diff, 1, patterns[pattern].elementsMask, type);
    }
}

void renderScene() {
    uint8_t lastPattern, lastIndex;
    uint8_t *baseScreen;
    int8_t *stencilPtr;
    unsigned char *pS;
    unsigned char *lastPS;
    unsigned char nByte;
    uint8_t y;
    
    switch (cameraRotation) {
        case DIRECTION_N: {
            int8_t y;
            int8_t limit = max(cameraZ - 19, 0);
            for ( y = min(cameraZ - 3, 31); y >= limit; --y) {
                int8_t x;
                int8_t *mapY = map[y];
                int8_t *mapXY;
                int8_t minX = cameraX + 5 + ((cameraZ - 3) - y);
                int8_t maxX = 0;

                if (minX > 31) {
                    minX = 31;
                }

                lastIndex = cameraX;
                lastPattern = *(mapY + lastIndex);

                mapXY = &map[y][lastIndex];
                for (x = lastIndex; x < minX - 1; ++x) {
                    uint8_t pattern;

                    pattern = *mapXY;

                    if (pattern != lastPattern) {
                        if (lastPattern != 0) {
                            if (!drawPattern(lastPattern, lastIndex - cameraX, x - cameraX, cameraZ - y)) {
                                x = minX - 1;
                            }
                            lastIndex = x;
                        }
                        lastPattern = pattern;
                    }

                    ++mapXY;
                }
                if (lastPattern != 0) {
                    drawPattern(lastPattern, lastIndex - cameraX, x - cameraX, cameraZ - y);
                }

                lastIndex = cameraX - 1;
                if (lastIndex < 0) {
                    lastIndex = 0;
                }

                lastPattern = *(mapY + lastIndex);

                mapXY = &map[y][lastIndex];

                maxX = max(cameraX - 3 - ((cameraZ - 3) - y), 0);

                for (x = lastIndex; x >= maxX + 1; --x) {
                    uint8_t pattern;
                    pattern = *mapXY;

                    if (pattern != lastPattern) {
                        if (lastPattern != 0) {

                            if (!drawPattern(lastPattern, x + 1 - cameraX, lastIndex + 1 - cameraX, cameraZ - y)) {
                                x = maxX + 1;
                            }

                            lastIndex = x;
                        }
                        lastPattern = pattern;
                    }

                    --mapXY;
                }
                if (lastPattern != 0) {
                    drawPattern(lastPattern, x + 1 - cameraX, lastIndex + 1 - cameraX, cameraZ - y);
                }
            }
        }
            break;

        case DIRECTION_E: {
            int8_t x;
            for ( x = min(cameraX - 3, 31); x <= min(cameraX + 13, 31); ++x) {
                int8_t y;

                for (y = cameraZ; y <= min(cameraZ + (x - cameraX), 31); ++y) {
                    drawPattern(map[y][x], y - cameraZ + 3, y + 1 - cameraZ + 3, x - cameraX + 3);
                }

                for (y = max(cameraZ - 1, 0); y >= max(cameraZ - (x - cameraX), 0); --y) {
                    drawPattern(map[y][x], y - cameraZ + 3, y + 1 - cameraZ + 3, x - cameraX + 3);
                }

            }
        }
            break;

        case DIRECTION_S: {
            int8_t y;
            for (y = min(cameraZ + 3, 31); y <= min(cameraZ + 19, 31); ++y) {
                int8_t x;
                for (x = cameraX; x <= min(cameraX + (y - (cameraZ + 3)), 31); ++x) {
                    drawPattern(map[y][x], cameraX - x, cameraX - x + 1, y - cameraZ);
                }

                for (x = max(cameraX - 1, 0); x >= max(cameraX - (y - (cameraZ + 3)), 0); --x) {
                    drawPattern(map[y][x], cameraX - x, cameraX - x + 1, y - cameraZ);
                }
            }
        }
            break;

        case DIRECTION_W: {
            int8_t x;
            for (x = max(cameraX, 0); x >= max(cameraX - 16, 0); --x) {
                int8_t y;
                for (y = cameraZ; y <= min(cameraZ - (x - (cameraX)), 31); ++y) {
                    drawPattern(map[y][x], y - cameraZ + 3, y + 1 - cameraZ + 3, cameraX - x + 1);
                }

                for (y = max(cameraZ - 1, 0); y >= max(cameraZ + (x - (cameraX)), 0); --y) {
                    drawPattern(map[y][x], y - cameraZ + 3, y + 1 - cameraZ + 3, cameraX - x + 1);
                }
            }
        }
            break;
    }
    
#ifdef CPC_PLATFORM
    baseScreen = (uint8_t *) (getFrame()) ? 0x8000 : 0xC000;
    stencilPtr = &stencilHigh[0];
#endif
    
    for (uint8_t x = 0; x < 127; ++x) {
#ifdef CPC_PLATFORM
        y = *stencilPtr;
        
        lastPS = (unsigned char *) baseScreen + lineStart[y];
        
        pS = lastPS + (x >> 1);
        nByte = *pS;
        
        nByte &= 85;
        nByte |= 128;
        
        *pS = nByte;
        
 
        ++x;
        ++stencilPtr;
        
        y = *stencilPtr;
        
        lastPS = (unsigned char *) baseScreen + lineStart[y];
        
        pS = lastPS + (x >> 1);
        nByte = *pS;
        
        nByte &= 170;
        nByte |= 64;
        
        *pS = nByte;
        ++stencilPtr;
#else
        graphicsPut( x, stencilHigh[x]);
#endif
    }
}

void tickRenderer() {
    uint8_t prevX;
    uint8_t prevZ;

#ifndef CPC_PLATFORM
    clearGraphics();
#endif
    renderScene();

    vLine(127, 0, 127);
    vLine(0, 0, 127);
//    hLine(0, 127, 0);
//    hLine(0, 127, 127);

    graphicsFlush();
    memset(stencilHigh, 0, 128);
#ifdef CPC_PLATFORM
    clearGraphics();
#endif
    prevX = cameraX;
    prevZ = cameraZ;

    waitkey:
    switch (getKey()) {
        case 'q':
            cameraRotation--;
            if (cameraRotation < 0) {
                cameraRotation = 3;
            }
            break;
        case 'e':
            cameraRotation = (cameraRotation + 1) & 3;
            break;
        case 'l':
            running = 0;
            break;

        case 'a':
            cameraX--;
            break;
        case 'd':
            cameraX++;
            break;


        case 's':
            switch (cameraRotation) {
                case 0:
                    cameraZ++;
                    break;
                case 1:
                    cameraX--;
                    break;
                case 2:
                    cameraZ--;
                    break;
                case 3:
                    cameraX++;
                    break;
            }


            break;
        case 'w':
            switch (cameraRotation) {
                case 0:
                    cameraZ--;
                    break;
                case 1:
                    cameraX++;
                    break;
                case 2:
                    cameraZ++;
                    break;
                case 3:
                    cameraX--;
                    break;
            }
            break;

#ifndef XCODE_BUILD
#if !defined(SDLSW) || !defined(AMIGA)
        default:
            goto waitkey;
#endif
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
}


#ifdef XCODE_BUILD
int demoMain() {
#else

int main(int argc, char **argv) {
#endif


#ifdef CPC_PLATFORM
    cpct_setStackLocation(0x8000);
#endif
    {
        running = 1;
        cameraX = 5;
        cameraZ = 15;
        cameraRotation = 0;
        init();

        memset(stencilHigh, 0, 128);

#ifndef XCODE_BUILD
        do {
            tickRenderer();
        } while (running);

        shutdownGraphics();
#endif
    }
    return 0;
}
