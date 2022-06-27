#ifdef MSX
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>


uint8_t getch();


void fix_line (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    draw(x0, y0, x1, y1 );
}

void shutdownGraphics() {
}

void writeStr(uint8_t nColumn, uint8_t nLine, char *str, uint8_t fg, uint8_t bg){}

void hLine(uint8_t x0, uint8_t x1, uint8_t y) {



    draw(x0, y, x1, y );
}

void vLine(uint8_t x0, uint8_t y0, uint8_t y1) {



    draw(x0, y0, x0, y1 );
}

void graphicsPut( uint8_t x, uint8_t y) {


    plot(x, y);
}

void clearGraphics() {
   clg();
}

uint8_t getKey() {
  return getch();
}

void init() {
    clg();
}

void graphicsFlush() {
}
#endif
