#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cpctelera.h>

#include "cpct_drw_mode0/cpct_drw_shared.h"
#include "cpct_drw_mode0/cpct_drw_mode0.h"

#define SCR_4000 (u8*)0x4000
#define SCR_C000 (u8*)0xC000

u8 page;

void shutdown() { 
}


u8 const g_palette[16]={
    0x00, // black
    0x1a, // brightWhite
    0x0d, // white
    0x12, // brightGreen
    0x06, // brightRed
    0x02, // brightBlue
    0x18, // brightYellow
    0x0f, // orange
    0x10, // pink
    0x11, // pastel magenta
    0x05, // mauve
    0x01, // blue
    0x00, // black
    0x00, // black
    0x00, // black
    0x00, // black
};

void clear() {
    cpct_memset_f64(SCR_C000,0x0000,16384);
}

uint8_t getKey() {
  
  cpct_scanKeyboard_f();

  if (cpct_isKeyPressed(Key_W))
    return 'w';

  if (cpct_isKeyPressed(Key_S))
    return 's';  
  
  if (cpct_isKeyPressed(Key_D))
    return 'd';

  if (cpct_isKeyPressed(Key_A))
    return 'a';


  
  return '.';
}

void init() {
  cpct_disableFirmware();

  cpct_fw2hw(g_palette,16);
  cpct_setPalette(g_palette,16);
  cpct_setBorder(g_palette[5]);

  cpct_drw_populateLineMasks_mode0();
  cpct_drw_setLineColour_mode0(1);
    
  cpct_memset_f64(SCR_C000,0x0000,16384);
    
  cpct_setVideoMemoryPage(cpct_pageC0);
  cpct_setVideoMode(0);
    
  page=0x80;
  cpct_drw_setClippingBox_mode0(0,159,0,199);
}

void graphicsFlush() {
    cpct_waitVSYNC();
}


void fix_line (int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    cpct_drw_line_mode0(x0 , y0, x1 , y1);
}

void graphicsPut( int x, int y ) {
  fix_line(x, y, x, y);
}
