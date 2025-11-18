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

#define CHARACTER_PIXEL_WIDTH 8
#define CHARACTER_PIXEL_HEIGHT 8

#define BLINK_EVERY_X_FRAMES 50
#define BLINK_FOR_X_FRAMES 10

N64Display::N64Display()
{
	display_init({640, 480, false}, DEPTH_32_BPP, DISPLAY_BUFFERS, GAMMA_NONE, FILTERS_DISABLED);
	uint32_t background(color_to_packed32(RGBA32(BG_COLOR_RED, BG_COLOR_GREEN, BG_COLOR_BLUE, 255)));
	uint32_t foreground(color_to_packed32(RGBA32(FG_COLOR_RED, FG_COLOR_GREEN, FG_COLOR_BLUE, 255)));
	graphics_set_color(foreground, background);
	currentFrame = display_get();
	blinkFrameCount = 0;
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
	blinkFrameCount = (blinkFrameCount + 1) % BLINK_EVERY_X_FRAMES;
	currentFrame = display_get();
	graphics_fill_screen(currentFrame, graphics_make_color(BG_COLOR_RED, BG_COLOR_GREEN, BG_COLOR_BLUE, 255));
}

void N64Display::drawBox(const Box& box)
{
	if (!currentFrame)
		return;
	char borderChar('*');
	for (int y = box.yPosition; y < box.yPosition+box.height; y++)
	{
		if (y == box.yPosition || y == box.yPosition+box.height-1)
		{
			for (int x = box.xPosition; x < box.xPosition+box.width+1; x++)
			{
				graphics_draw_character(currentFrame, x*CHARACTER_PIXEL_WIDTH, y*CHARACTER_PIXEL_HEIGHT, borderChar);
			}
			continue;
		}
		graphics_draw_character(currentFrame, box.xPosition*CHARACTER_PIXEL_WIDTH, y*CHARACTER_PIXEL_HEIGHT, borderChar);
		graphics_draw_character(currentFrame, (box.xPosition+box.width)*CHARACTER_PIXEL_WIDTH, y*CHARACTER_PIXEL_HEIGHT, borderChar);
	}
}

void N64Display::drawTextBox(const TextBox& textBox)
{
	if (textBox.isBlinking() && blinkFrameCount >= BLINK_EVERY_X_FRAMES-BLINK_FOR_X_FRAMES)
		return;
	if (textBox.isBordered())
		drawBox(textBox);
	graphics_draw_text(currentFrame, (textBox.xPosition+1+(textBox.width-textBox.text.size())/2)*CHARACTER_PIXEL_WIDTH, (textBox.yPosition+textBox.height/2)*CHARACTER_PIXEL_HEIGHT,textBox.text.c_str());
}

void N64Display::drawQrBox(const QrBox& qrBox)
{
	const uint32_t foregroundColor = graphics_make_color(FG_COLOR_RED, FG_COLOR_GREEN, FG_COLOR_BLUE, 255);
	for (int y = 0; y < qrBox.height; y++)
	{
		for (int x = 0; x < qrBox.width; x++)
		{
			if (!qrBox.qrCode.getModule(x, y))
				continue;
			for (int j = 0; j < CHARACTER_PIXEL_HEIGHT; j++)
				for (int i = 0; i < CHARACTER_PIXEL_WIDTH; i++)
					graphics_draw_pixel(currentFrame, (qrBox.xPosition+x)*CHARACTER_PIXEL_WIDTH+i, (qrBox.yPosition+y)*CHARACTER_PIXEL_HEIGHT+j, foregroundColor);
		}
	}
}
