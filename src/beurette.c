#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IN_RANGE(V0, V1, V)  ((V0) <= (V) && (V) <= (V1))

void shutdown();

void writeStr(uint8_t nColumn, uint8_t nLine, char *str, uint8_t fg, uint8_t bg);

void graphicsPut(uint8_t x, uint8_t y);

void clear();

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

struct Projection {
    uint8_t px;
    uint8_t py;
    int16_t dx;
};


struct Pattern {
    int8_t ceiling : 4;
    uint8_t elementsMask: 4;
    uint8_t geometryType;
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

const struct Pattern patterns[16] = {
        {5,  3, 0}, //0
        {0,  3, 0}, // 1
        {-1, 0, 0}, // 2
        {-1, 3, 0}, //3
        {-1, 3, 4}, //4
        {-1, 3, 8}, //5
        {2,  3, 0}, //6
        {5,  3, 0}, //7
        {-1, 1, 0}, // 8
        {-1, 3, 0}, // 9
};

const int8_t map[32][32] = {
        {0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x8, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x8, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x6, 0x7, 0x7, 0x7, 0x3, 0x3, 0x3, 0x3, 0x7, 0x8, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x8, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x2, 0x2, 0x2, 0x8, 0x2, 0x2, 0x2, 0x7, 0x3, 0x6, 0x7, 0x7, 0x3, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x7, 0x7, 0x5, 0x3, 0x6, 0x6, 0x3, 0x4, 0x7, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x6, 0x7, 0x7, 0x3, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x8, 0x7, 0x7, 0x6, 0x7, 0x7, 0x3, 0x7, 0x7, 0x6, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x4, 0x2, 0x7, 0x7, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x3, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x8, 0x6, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x3, 0x2, 0x7, 0x7, 0x9, 0x7, 0x7, 0x7, 0x7, 0x7, 0x3, 0x7, 0x7, 0x7, 0x7, 0x9, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x3, 0x6, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x6, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x8, 0x7, 0x7, 0x3, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x8, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x1, 0x6, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x6, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x3, 0x6, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x3, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x6, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x5, 0x2, 0x2, 0x2, 0x2, 0x2, 0x9, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x8, 0x7, 0x7, 0x7, 0x7, 0x7, 0x9, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x8, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x5, 0x4, 0x7, 0x7, 0x9, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x8, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x9, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x8, 0x7, 0x7, 0x7, 0x7, 0x3, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x9, 0x2, 0x2, 0x2, 0x9, 0x7, 0x7, 0x7, 0x7, 0x7, 0x3, 0x2, 0x2, 0x9, 0x7, 0x7, 0x7, 0x8, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x9, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x9, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x8, 0x7, 0x7, 0x7, 0x7, 0x7, 0x8, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x7, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7, 0x7, 0x7, 0x7, 0x8, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7},
        {0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x7}
};

int8_t max(int8_t x1, int8_t x2) {
    return x1 > x2 ? x1 : x2;
}

int8_t min(int8_t x1, int8_t x2) {
    return x1 < x2 ? x1 : x2;
}


void drawWedge(int8_t x0, int8_t y0, int8_t z0, int8_t dX, int8_t dY, int8_t dZ, uint8_t elementMask, uint8_t type) {

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

    uint8_t drawContour;

    if (z0 >= 16) {
        z0 = 15;
    }

    z1 = z0 + dZ;

    if (z0 <= 3) {
        return;
    }

    if (z1 <= 3) {
        return;
    }

    if (z1 >= 16) {
        z1 = 15;
    }


    if (type == 4) {
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

    drawContour = 1;

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

        if (drawContour) {
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
        }



        /* The upper segment */
        x0 = px0z0;
        x1 = px1z1;

        if (x0 != x1) {
            int16_t y0 = py1z0;
            int16_t y1 = py1z1;
            int16_t dx = abs(x1 - x0);
            int16_t sx = x0 < x1 ? 1 : -1;
            int16_t dy = -abs(y1 - y0);
            int16_t sy = y0 < y1 ? 1 : -1;
            int16_t err = dx + dy;  /* error value e_xy */
            int16_t e2;

            while ((x0 != x1 || y0 != y1)) {

                if (IN_RANGE(0, 127, x0)) {
                    if (stencilHigh[x0] <= y0) {
                        if (drawContour) {
                            graphicsPut(x0, y0);
                        }
                    }
                }

                /* loop */
                e2 = err << 2;

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

        /* The lower segment */
        x0 = px0z0;
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

                if (IN_RANGE(0, 127, x0)) {
                    if (stencilHigh[x0] < y0) {
                        if (drawContour) {
                            graphicsPut(x0, y0);
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

                if (e2 <= dx) {
                    /* e_xy+e_y < 0 */
                    err += dx;
                    y0 += sy;
                }
            }
        }
    }
}

void drawCubeAt(int8_t x0, int8_t y0, int8_t z0, int8_t dX, int8_t dY, int8_t dZ, uint8_t elementMask) {

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

    px1z0 = px0z0 - (dX * z0dx);
    px1z1 = px0z1 - (dX * z1dx);

    z1py = (projections[z1].py);
    z0py = (projections[z0].py);

    py0z0 = z0py + ((y0) * z0dx);
    py1z0 = py0z0 + (dY * z0dx);
    py0z1 = z1py + ((y0) * z1dx);

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
                        graphicsPut(x, py0z0);
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
                    graphicsPut(x, py0z0);
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
                            graphicsPut(x0, y0);
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
                        graphicsPut(x0, y0);
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
                    if (drawContour) {
                        graphicsPut(x, py0z1);
                    }
                    stencilHigh[x] = py0z1;
                }
            }
        }
    }
}

void drawPattern(uint8_t pattern, uint8_t x0, uint8_t x1, uint8_t y) {

    int8_t diff = patterns[0].ceiling - patterns[pattern].ceiling;
    uint8_t type = patterns[pattern].geometryType;

    if (type == 0) {
        drawCubeAt(x0, patterns[pattern].ceiling, y, x1 - x0,
                   diff, 1, patterns[pattern].elementsMask);

    } else {
        if (cameraRotation == 1 || cameraRotation == 2) {
            if (type == 4) {
                type = 8;
            } else {
                type = 4;
            }
        }

        drawWedge(x0, patterns[pattern].ceiling, y, x1 - x0,
                  diff, 1, patterns[pattern].elementsMask, type);
    }
}

void renderScene() {
    uint8_t lastPattern, lastIndex;

    switch (cameraRotation) {
        case 0: {

            int8_t limit = max(cameraZ - 19, 0);
            for (int8_t y = min(cameraZ - 3, 31); y >= limit; --y) {
                int8_t x;
                lastIndex = cameraX;
                lastPattern = map[y][lastIndex];

                for (x = lastIndex; x < min(cameraX + 5 + ((cameraZ - 3) - y), 31); ++x) {
                    uint8_t pattern;

                    pattern = map[y][x];

                    if (pattern != lastPattern) {
                        if (lastPattern != 0) {
                            drawPattern(lastPattern, lastIndex - cameraX, x - cameraX, cameraZ - y);
                            lastIndex = x;
                        }
                        lastPattern = pattern;
                    }
                }
                if (lastPattern != 0) {
                    drawPattern(lastPattern, lastIndex - cameraX, x - cameraX, cameraZ - y);
                }

                lastIndex = max(cameraX - 1, 0);
                lastPattern = map[y][lastIndex];

                for (x = lastIndex; x >= max(cameraX - 3 - ((cameraZ - 3) - y), 0); --x) {
                    uint8_t pattern;
                    pattern = map[y][x];

                    if (pattern != lastPattern) {
                        if (lastPattern != 0) {
                            drawPattern(lastPattern, x + 1 - cameraX, lastIndex + 1 - cameraX, cameraZ - y);
                            lastIndex = x;
                        }
                        lastPattern = pattern;
                    }
                }
                if (lastPattern != 0) {
                    drawPattern(lastPattern, x + 1 - cameraX, lastIndex + 1 - cameraX, cameraZ - y);
                }
            }
        }
            break;

        case 1: {

            for (int8_t x = min(cameraX + 3, 31); x <= min(cameraX + 19, 31); ++x) {
                int8_t y;

                for (y = cameraZ; y <= min(cameraZ + (x - (cameraX + 3)), 31); ++y) {
                    drawPattern(map[y][x], y - cameraZ, y + 1 - cameraZ, x - cameraX);
                }

                for (y = max(cameraZ - 1, 0); y >= max(cameraZ - (x - (cameraX + 3)), 0); --y) {
                    drawPattern(map[y][x], y - cameraZ, y + 1 - cameraZ, x - cameraX);
                }

            }
        }
            break;

        case 2: {

            for (int8_t y = min(cameraZ + 3, 31); y <= min(cameraZ + 19, 31); ++y) {
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

        case 3: {

            for (int8_t x = max(cameraX - 3, 0); x >= max(cameraX - 19, 0); --x) {
                int8_t y;
                for (y = cameraZ; y <= min(cameraZ - (x - (cameraX - 3)), 31); ++y) {
                    drawPattern(map[y][x], y - cameraZ, y + 1 - cameraZ, cameraX - x);
                }

                for (y = max(cameraZ - 1, 0); y >= max(cameraZ + (x - (cameraX - 3)), 0); --y) {
                    drawPattern(map[y][x], y - cameraZ, y + 1 - cameraZ, cameraX - x);
                }
            }
        }
            break;
    }
}

int main(int argc, char **argv) {
    uint8_t running = 1;
    uint8_t prevX;
    uint8_t prevZ;
    cameraX = 5;
    cameraZ = 15;
    cameraRotation = 0;
    init();

    memset(stencilHigh, 0, 128);


    do {
        clear();
        renderScene();

        vLine(127, 0, 127);
        vLine(0, 0, 127);
        hLine(0, 127, 0);
        hLine(0, 127, 127);

        graphicsFlush();
        memset(stencilHigh, 0, 128);

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
                cameraZ++;
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
