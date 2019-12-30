#ifdef SDLSW

#include <stdio.h>
#include <stdlib.h>


#include "SDL.h"
#include "FixP.h"

SDL_Window *window;
SDL_Renderer *renderer;

#define TRANSPARENCY_COLOR 17

uint8_t mBufferedCommand;
uint32_t palette[16];
uint8_t framebuffer[160 * 200];
void graphicsFlush();

void graphicsPut(int x, int y, int colour) {

	x = x / 2;

	if (x < 0 || x > 160 || y < 0 || y > 200) {
		return;
	}



	framebuffer[(160 * y) + x] = colour;
#ifdef PUTAFLIP
	graphicsFlush();
	SDL_Delay(100);
#endif
}


void fix_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour) {

	if (x0 == x1) {

		int16_t _y0 = y0;
		int16_t _y1 = y1;

		if (y0 > y1) {
			_y0 = y1;
			_y1 = y0;
		}


		for (int16_t y = _y0; y <= _y1; ++y) {
			if (x0 < 0 || x0 >= 256 || y < 0 || y >= 128) {
				continue;
			}

			graphicsPut(x0, y, colour);
		}
		return;
	}

	if (y0 == y1) {
		int16_t _x0 = x0;
		int16_t _x1 = x1;

		if (x0 > x1) {
			_x0 = x1;
			_x1 = x0;
		}

		for (int16_t x = _x0; x <= _x1; ++x) {
			if (x < 0 || x >= 256 || y0 < 0 || y0 >= 128) {
				continue;
			}

			graphicsPut(x, y0, colour);
		}
		return;
	}

	//switching x0 with x1
	if (x0 > x1) {
		x0 = x0 + x1;
		x1 = x0 - x1;
		x0 = x0 - x1;

		y0 = y0 + y1;
		y1 = y0 - y1;
		y0 = y0 - y1;
	}

	{
		//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

		int dx = abs(x1 - x0);
		int sx = x0 < x1 ? 1 : -1;
		int dy = -abs(y1 - y0);
		int sy = y0 < y1 ? 1 : -1;
		int err = dx + dy;  /* error value e_xy */

		while (1) {
			graphicsPut(x0, y0, colour);
			/* loop */
			if (x0 == x1 && y0 == y1) return;
			int e2 = 2 * err;

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

void hLine(int16_t x0, int16_t x1, int16_t y, uint8_t colour) {
	fix_line(x0, y, x1, y, colour );
}

void vLine(int16_t x0, int16_t y0, int16_t y1, uint8_t colour) {
	fix_line(x0, y0, x0, y1, colour );
}


void graphicsFill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour) {

}


void shutdown() {
	SDL_Quit();
}


void clear() {
	memset(framebuffer, 0, 160 * 200);
}

uint8_t getKey() {
	SDL_Event event;

	mBufferedCommand = '.';

	while (SDL_PollEvent(&event)) {

		if (event.type == SDL_QUIT) {

			mBufferedCommand = 'q';
		}

		if (event.type == SDL_KEYUP) {

			switch (event.key.keysym.sym) {
				case SDLK_RETURN:
				case SDLK_z:
					break;

				case SDLK_ESCAPE:
				case SDLK_q:
					mBufferedCommand = 'q';
					break;

				case SDLK_SPACE:
				case SDLK_s:
					break;
				case SDLK_d:
					break;
				case SDLK_v:
					break;
				case SDLK_b:
					break;
				case SDLK_j:
					break;
				case SDLK_k:
					break;
				case SDLK_x:
					break;
				case SDLK_c:
					break;
				case SDLK_e:
					break;

				case SDLK_LEFT:
					mBufferedCommand = 'a';
					break;
				case SDLK_RIGHT:
					mBufferedCommand = 'd';
					break;
				case SDLK_UP:
					mBufferedCommand = 'w';
					break;
				case SDLK_DOWN:
					mBufferedCommand = 's';
					break;

				default:
					return '.';
			}
		}
	}

	return mBufferedCommand;
}

void init() {
	int r, g, b;
	mBufferedCommand = '.';
	SDL_Init(SDL_INIT_EVERYTHING);
	memset(framebuffer, 5, 160 * 200);
	window =
			SDL_CreateWindow("The Mistral Report", SDL_WINDOWPOS_CENTERED,
							 SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, 0);

	palette[0] = 0xFF000099;
	palette[1] = 0xFFFFFFBF;
	palette[2] = 0xFFE0FFFF;
	palette[3] = 0xFFFF0000;
	palette[4] = 0xFFFFFFFF;
	palette[5] = 0xFF000000;
	palette[6] = 0xFF0000FF;
	palette[7] = 0xFFFF00FF;
	palette[8] = 0xFF00b7eb;
	palette[9] = 0xFFFFFF00;
	palette[10] = 0xFFAFEEEE;
	palette[11] = 0xFFffc0cb;
	palette[12] = 0xFF00FF00;
	palette[13] = 0xFFAAFFAA;
	palette[14] = 0xFF0000FF;
	palette[15] = 0xFFAAAAFF;

#ifdef __EMSCRIPTEN__
	enterFullScreenMode ();
#endif


}


void flipRenderer() {
	SDL_Rect rect;
	uint32_t pixel;
	int x, y;

	for (y = 0; y < 200; ++y) {
		for (x = 0; x < 160; ++x) {

			rect.x = 4 * x;
			rect.y = (24 * y) / 10;
			rect.w = 4;
			rect.h = 3;
			int index = framebuffer[(160 * y) + x];

			if (index < 0 || index >= 16 ) {
				continue;
			}

			pixel = palette[index];

			int r = (pixel & 0x00FF0000) >> 16;
			int g = ((pixel & 0x0000FF00) >> 8);
			int b = ((pixel & 0x000000FF));

			SDL_SetRenderDrawColor(renderer, r,
								   g,
								   b, 255);
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	SDL_RenderPresent(renderer);

#ifndef __EMSCRIPTEN__
	SDL_Delay(1000 / 60);
#endif
}

void graphicsFlush() {
	flipRenderer();
}

#endif
