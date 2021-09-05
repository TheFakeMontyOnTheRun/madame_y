#ifdef GB


#include <gb/gb.h>
#include <gb/drawing.h>

#define XRES_SCREEN (160 - 1)

#define YRES_SCREEN (144 - 1)

void fix_line (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
}

void shutdownGraphics() {
}

void writeStr(uint8_t nColumn, uint8_t nLine, char *str, uint8_t fg, uint8_t bg){}

void hLine(uint8_t x0, uint8_t x1, uint8_t y) {

    if (y > YRES_SCREEN ) {
        return;
    }

    if (x1 > XRES_SCREEN ) {
        x1 = XRES_SCREEN;
    }

    if (x0 > XRES_SCREEN ) {
        x0 = XRES_SCREEN;
    }


    line(x0, y, x1, y );
}

void vLine(uint8_t x0, uint8_t y0, uint8_t y1) {

    if (y1 > YRES_SCREEN ) {
        y1 = YRES_SCREEN;
    }

    if (y0 > YRES_SCREEN ) {
        y0 = YRES_SCREEN;
    }

    if (x0 > XRES_SCREEN ) {
        return;
    }

    line(x0, y0, x0, y1 );
}

void graphicsPut( uint8_t x, uint8_t y) {
    if (y > YRES_SCREEN ) {
        return;
    }

    if (x > XRES_SCREEN ) {
        return;
    }


    plot(x, y, BLACK, SOLID);
}

void clearGraphics() {
    box( 0, 0, XRES_SCREEN, YRES_SCREEN, M_FILL);
}

uint8_t getKey() {
    wait_vbl_done();
    int key = joypad();
    if(key & (J_UP|J_DOWN|J_LEFT|J_RIGHT)) {

        if(key & J_UP) {
            return 'w';
        }

        if(key & J_DOWN) {
            return 's';
        }

        if(key & J_LEFT) {
            return 'a';
        }

        if(key & J_RIGHT) {
            return 'd';
        }
    }
  return '.';
}

void init() {
    OBP1_REG = 0xE0U;
}

void graphicsFlush() {
    wait_vbl_done();

}
#endif
