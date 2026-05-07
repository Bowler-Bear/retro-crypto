#ifndef SCALED_CHARACTER_DRAWING_H
#define SCALED_CHARACTER_DRAWING_H

#include <surface.h>

#ifndef BG_COLOR_RED
#define BG_COLOR_RED DEFAULT_BG_COLOR_RED
#endif
#ifndef BG_COLOR_GREEN
#define BG_COLOR_GREEN DEFAULT_BG_COLOR_GREEN
#endif
#ifndef BG_COLOR_BLUE
#define BG_COLOR_BLUE DEFAULT_BG_COLOR_BLUE
#endif
#ifndef FG_COLOR_RED
#define FG_COLOR_RED DEFAULT_FG_COLOR_RED
#endif
#ifndef FG_COLOR_GREEN
#define FG_COLOR_GREEN DEFAULT_FG_COLOR_GREEN
#endif
#ifndef FG_COLOR_BLUE
#define FG_COLOR_BLUE DEFAULT_FG_COLOR_BLUE
#endif

#define MAXIMUM_UNIFONT_CHARACTER_PIXEL_WIDTH 16
#define UNIFONT_CHARACTER_PIXEL_HEIGHT 16
#define MAXIMUM_UNIFONT_BITMAP_SIZE (MAXIMUM_UNIFONT_CHARACTER_PIXEL_WIDTH/8)*UNIFONT_CHARACTER_PIXEL_HEIGHT

void graphics_draw_scaled_character( surface_t* disp, int x, int y, char ch, int scale);

void graphics_draw_scaled_unifont_bitmap(surface_t* disp, int x, int y, char characterBitmap[MAXIMUM_UNIFONT_BITMAP_SIZE], int characterWidth, int scale);

#endif
