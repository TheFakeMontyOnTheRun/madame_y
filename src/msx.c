#ifdef MSX
#include <lib3d.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <msx/gfx.h>



uint8_t getch();


unsigned char* sbuffer;
surface_t surf;

void fix_line (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t colour) {
 // surface_draw(&surf, x0, y0, x1, y1 );
 line( 2 * x0, y0, 2 * x1, y1 );
}

void shutdown() {
  // go back to text mode
  set_mode(mode_0);
}

void hLine(uint8_t x0, uint8_t x1, uint8_t y, uint8_t colour) {

	if (y > 127 ) {
		return;
	}

	if (x1 > 127 ) {
		x1 = 127;
	}

	if (x0 > 127 ) {
		x0 = 127;
	}



	fix_line(x0, y, x1, y, colour );
}

void vLine(uint8_t x0, uint8_t y0, uint8_t y1, uint8_t colour) {

	if (y1 > 127 ) {
		y1 = 127;
	}

	if (y0 > 127 ) {
		y0 = 127;
	}


	if (x0 > 127 ) {
		return;
	}


	fix_line(x0, y0, x0, y1, colour );
}

void graphicsPut( uint8_t x, uint8_t y, uint8_t colour ) {
	if (y > 127 ) {
		return;
	}

	if (x > 127 ) {
		return;
	}

    pset( 2 * x, y );
	pset( 2 * x + 1, y );
    //fix_line( x, y, x, y, 1);
}

void clear() {
   set_mode(mode_2);
}

uint8_t getKey() {
  return getch();
}

void init() {
	sbuffer = (unsigned char*)malloc(MODE2_MAX);
	surf.data.ram = sbuffer;
	set_color(15, 1, 1);
	set_mode(mode_2);
	fill(MODE2_ATTR, 0xF1, MODE2_MAX);
}

void graphicsFlush() {
  /* msx_vwrite_direct(sbuffer, 0, MODE2_MAX); */
}
#endif
