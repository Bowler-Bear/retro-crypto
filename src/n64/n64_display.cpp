#include <display.h>
#include <graphics.h>

#include "n64_display.h"

#define DISPLAY_BUFFERS 2

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

N64Display::N64Display()
{
	display_init(RESOLUTION_640x480, DEPTH_32_BPP, DISPLAY_BUFFERS, GAMMA_NONE, FILTERS_DISABLED);
	uint32_t background(color_to_packed32(RGBA32(BG_COLOR_RED, BG_COLOR_GREEN, BG_COLOR_BLUE, 255)));
	uint32_t foreground(color_to_packed32(RGBA32(FG_COLOR_RED, FG_COLOR_GREEN, FG_COLOR_BLUE, 255)));
	graphics_set_color(foreground, background);
	currentFrame = display_get();
	blinkFrame = false;
}

N64Display::~N64Display()
{
	display_close();
}

void N64Display::redraw()
{
}

void N64Display::clear()
{
	display_show(currentFrame);
	blinkFrame = !blinkFrame;
	currentFrame = display_get();
}

void N64Display::drawBox(const Box& box)
{
	surface_t* currentDisplay = display_get();
	if (!currentDisplay)
		return;
	char borderChar('*');
	for (int y = box.yPosition; y < box.yPosition+box.height; y++)
	{
		if (y == box.yPosition || y == box.yPosition+box.height)
		{
			for (int x = box.xPosition; x < box.xPosition+box.width; x++)
			{
				graphics_draw_character(currentDisplay, x, y, borderChar);
			}
			continue;
		}
		graphics_draw_character(currentDisplay, box.xPosition, y, borderChar);
		graphics_draw_character(currentDisplay, box.xPosition+box.width, y, borderChar);
	}
}

void N64Display::drawTextBox(const TextBox& textBox)
{
	if (textBox.isBlinking() && blinkFrame == true)
		return;
	if (textBox.isBordered())
		drawBox(textBox);
	graphics_draw_text(currentFrame, textBox.xPosition+1+(textBox.width-textBox.text.size())/2, textBox.yPosition+1+textBox.height/2,textBox.text.c_str());
}
