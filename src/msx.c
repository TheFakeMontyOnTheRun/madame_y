#ifdef MSX

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <msx/gfx.h>




uint8_t getch();


surface_t surface;
unsigned char* sbuffer;

void fix_line (int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour) {
  surface_draw(&surface, x0, y0, x1, y1 );
}


void shutdown() {
  // go back to text mode
  set_mode(mode_0);
}

void hLine(int16_t x0, int16_t x1, int16_t y, uint8_t colour) {
	fix_line(x0, y, x1, y, colour );
}

void vLine(int16_t x0, int16_t y0, int16_t y1, uint8_t colour) {
	fix_line(x0, y0, x0, y1, colour );
}

void graphicsPut( int x, int y, uint8_t colour ) {
  fix_line( x, y, x, y, 1);
}

void clear() {
   memset(sbuffer, 0, 32*128);  // [*]
}

uint8_t getKey() {
  return getch();
}

void init() {
  sbuffer = (unsigned char*)malloc(MODE2_MAX);
  surface.data.ram = sbuffer;
  set_mode(mode_2);
  msx_vfill(MODE2_ATTR, 0xf1, MODE2_MAX);
  memset(sbuffer, 0, MODE2_MAX);  // [*]
}

void graphicsFlush() {
  msx_vwrite_direct(sbuffer, 0, MODE2_MAX);
}
#endif
