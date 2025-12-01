#include <display.h>
#include <graphics.h>

#include "n64_display.h"
#include "scaled_character_drawing.h"

#define DISPLAY_BUFFERS 2

#ifndef N64_CHARACTER_PIXEL_WIDTH
#define N64_CHARACTER_PIXEL_WIDTH 8
#endif
#ifndef N64_CHARACTER_PIXEL_HEIGHT
#define N64_CHARACTER_PIXEL_HEIGHT 8
#endif

#define BLINK_EVERY_X_FRAMES 50
#define BLINK_FOR_X_FRAMES 10

#ifndef N64_RESOLUTION_WIDTH
#define N64_RESOLUTION_WIDTH 640
#endif
#ifndef N64_RESOLUTION_HEIGHT
#define N64_RESOLUTION_HEIGHT 480
#endif

#ifndef BASE_BORDER_BOX_WIDTH
#define BASE_BORDER_BOX_WIDTH ((N64_RESOLUTION_WIDTH / N64_CHARACTER_PIXEL_WIDTH) - 1)
#endif
#ifndef BASE_BORDER_BOX_HEIGHT
#define BASE_BORDER_BOX_HEIGHT ((N64_RESOLUTION_HEIGHT / N64_CHARACTER_PIXEL_HEIGHT) - 1)
#endif

N64Display::N64Display()
{
	display_init({N64_RESOLUTION_WIDTH, N64_RESOLUTION_HEIGHT, false}, DEPTH_32_BPP, DISPLAY_BUFFERS, GAMMA_NONE, FILTERS_DISABLED);
	uint32_t background(color_to_packed32(RGBA32(BG_COLOR_RED, BG_COLOR_GREEN, BG_COLOR_BLUE, 255)));
	uint32_t foreground(color_to_packed32(RGBA32(FG_COLOR_RED, FG_COLOR_GREEN, FG_COLOR_BLUE, 255)));
	graphics_set_color(foreground, background);
	currentFrame = display_get();
	blinkFrameCount = 0;
	characterScale = 1;
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
				drawCharacter(x, y, borderChar);
			}
			continue;
		}
		drawCharacter(box.xPosition, y, borderChar);
		drawCharacter(box.xPosition+box.width, y, borderChar);
	}
}

void N64Display::drawTextBox(const TextBox& textBox)
{
	if (textBox.isBlinking() && blinkFrameCount >= BLINK_EVERY_X_FRAMES-BLINK_FOR_X_FRAMES)
		return;
	if (textBox.isBordered())
		drawBox(textBox);
	for (int i = 0; i < (int)textBox.text.length(); i++)
	{
		drawCharacter(textBox.xPosition+1+i+(textBox.width-(int)textBox.text.length())/2, textBox.yPosition+textBox.height/2, textBox.text[i]);
	}
}

void N64Display::drawQrBox(const QrBox& qrBox)
{
	const uint32_t foregroundColor = graphics_make_color(FG_COLOR_RED, FG_COLOR_GREEN, FG_COLOR_BLUE, 255);
	const int squareWidth = (BASE_BORDER_BOX_HEIGHT-1)*characterScale*N64_CHARACTER_PIXEL_HEIGHT/qrBox.height;
	const int startX = (BASE_BORDER_BOX_WIDTH*characterScale*N64_CHARACTER_PIXEL_WIDTH-squareWidth*qrBox.width)/2;
	const int startY = (BASE_BORDER_BOX_HEIGHT*characterScale*N64_CHARACTER_PIXEL_HEIGHT-squareWidth*qrBox.height)/2;
	for (int y = 0; y < qrBox.height; y++)
	{
		for (int x = 0; x < qrBox.width; x++)
		{
			if (!qrBox.qrCode.getModule(x, y))
				continue;
			for (int j = 0; j < squareWidth; j++)
				for (int i = 0; i < squareWidth; i++)
					if (isPositionVisible((startX+x*squareWidth+i)/(characterScale*N64_CHARACTER_PIXEL_WIDTH), (startY+y*squareWidth+j)/(characterScale*N64_CHARACTER_PIXEL_HEIGHT)))
						graphics_draw_pixel(currentFrame, startX+x*squareWidth+i, startY+y*squareWidth+j, foregroundColor);
		}
	}
}

bool N64Display::isPositionVisible(const int x, const int y)
{
	return x >= 0 && y >= 0 && x <= BASE_BORDER_BOX_WIDTH/characterScale && y < BASE_BORDER_BOX_HEIGHT/characterScale;
}

void N64Display::drawCharacter(const int x, const int y, const char character)
{
	if (isPositionVisible(x, y))
	{
		graphics_draw_scaled_character(currentFrame, x*characterScale*N64_CHARACTER_PIXEL_WIDTH, y*characterScale*N64_CHARACTER_PIXEL_HEIGHT, character, characterScale);
	}
}
