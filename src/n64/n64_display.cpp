#include <cstring>
#include <display.h>
#include <graphics.h>
#include <memory>

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
	viewportOffsetX = 0;
	viewportOffsetY = 0;
	characterScale = 1;
	unifont = nullptr;
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
	int displayedCharacters = 0;
	uint8_t* characterSizes = (uint8_t*) malloc(textBox.text.length());
	for (int i = 0; i < (int)textBox.text.length(); i++)
	{
		displayedCharacters++;
		characterSizes[i] = 1;
		int expectedCodePoints = 0;
		if ((textBox.text[i] & 0x80) == 0)
		{
			continue;
		}
		else if ((textBox.text[i] & 0xe0) == 0xc0)
		{
			expectedCodePoints = 1;
		}
		else if ((textBox.text[i] & 0xf0) == 0xe0)
		{
			expectedCodePoints = 2;
		}
		else if ((textBox.text[i] & 0xf8) == 0xf0)
		{
			expectedCodePoints = 3;
		}
		int startI = i;
		int finalIndex = i+1+expectedCodePoints;
		for (int j = i+1; j < finalIndex; j++)
		{
			if (j < (int)textBox.text.length() && (textBox.text[j] & 0xc0) == 0x80)
			{
				characterSizes[startI]++;
				characterSizes[j] = 0;
				i++;
				continue;
			}
			break;
		}
		unsigned char utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER] = { 0 };
		memcpy(utf8, &textBox.text[startI], characterSizes[startI]);
		if (unifont)
		{
			displayedCharacters += (unifont->getCharacterWidthFromUTF8(utf8)/N64_CHARACTER_PIXEL_WIDTH)-1;
		}
	}
	int startX = textBox.xPosition+1+((textBox.width-displayedCharacters)/2);
	displayedCharacters = 0;
	for (int i = 0; i < (int)textBox.text.length(); i++)
	{
		if (characterSizes[i] == 1)
		{
			drawCharacter(startX+displayedCharacters, textBox.yPosition+textBox.height/2, textBox.text[i]);
			displayedCharacters += 1;
		}
		else if (characterSizes[i] > 1)
		{
			unsigned char utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER] = { 0 };
			memcpy(utf8, &textBox.text[i], characterSizes[i]);
			int drawnCharacterWidth = drawUnifontCharacter(startX+displayedCharacters, textBox.yPosition+textBox.height/2, utf8);
			displayedCharacters += drawnCharacterWidth/N64_CHARACTER_PIXEL_WIDTH;
		}
	}
	free(characterSizes);
}

void N64Display::drawQrBox(const QrBox& qrBox)
{
	const uint32_t foregroundColor = graphics_make_color(FG_COLOR_RED, FG_COLOR_GREEN, FG_COLOR_BLUE, 255);
	const int squareWidth = (BASE_BORDER_BOX_HEIGHT-2)*characterScale*N64_CHARACTER_PIXEL_HEIGHT/qrBox.height;
	const int startX = (BASE_BORDER_BOX_WIDTH*characterScale*N64_CHARACTER_PIXEL_WIDTH-squareWidth*qrBox.width)/2;
	const int startY = (BASE_BORDER_BOX_HEIGHT*characterScale*N64_CHARACTER_PIXEL_HEIGHT-squareWidth*qrBox.height)/2;
	for (int y = 0; y < qrBox.height; y++)
	{
		for (int x = 0; x < qrBox.width; x++)
		{
			if (!qrBox.qrCode.getModule(x, y))
				continue;
			for (int j = 0; j < squareWidth; j++)
			{
				for (int i = 0; i < squareWidth; i++)
				{
					int pixelX = startX+x*squareWidth-viewportOffsetX*characterScale*N64_CHARACTER_PIXEL_WIDTH+i;
					int pixelY = startY+y*squareWidth-viewportOffsetY*characterScale*N64_CHARACTER_PIXEL_HEIGHT+j;
					if (pixelX >= 0 && pixelY >= 0 && pixelX < N64_RESOLUTION_WIDTH && pixelY < N64_RESOLUTION_HEIGHT)
						graphics_draw_pixel(currentFrame, pixelX, pixelY, foregroundColor);
				}
			}
		}
	}
}

bool N64Display::isPositionVisible(const int x, const int y)
{
	return x >= 0 && y >= 0 && x <= BASE_BORDER_BOX_WIDTH/characterScale && y < BASE_BORDER_BOX_HEIGHT/characterScale;
}

void N64Display::setUnifontHandler(std::shared_ptr<UnifontHandler> newUnifontHandler)
{
	unifont = newUnifontHandler;
}

void N64Display::drawCharacter(const int x, const int y, const char character)
{
	if (isPositionVisible(x-viewportOffsetX, y-viewportOffsetY))
	{
		graphics_draw_scaled_character(currentFrame, (x-viewportOffsetX)*characterScale*N64_CHARACTER_PIXEL_WIDTH, (y-viewportOffsetY)*characterScale*N64_CHARACTER_PIXEL_HEIGHT, character, characterScale);
	}
}

int N64Display::drawUnifontCharacter(const int x, const int y, const unsigned char utf8[MAXIMUM_UTF8_BYTES_PER_CHARACTER])
{
	if (isPositionVisible(x-viewportOffsetX, y-viewportOffsetY))
	{
		if (!unifont)
		{
			drawCharacter(x, y, '?');
			return N64_CHARACTER_PIXEL_WIDTH;
		}
		char bitmap[UNIFONT_16_WIDTH_BITMAP_SIZE] = { 0 };
		uint8_t characterWidth = unifont->getBitmapFromUTF8((const uint8_t*)utf8, (uint8_t*)bitmap);
		graphics_draw_scaled_unifont_bitmap(currentFrame, (x-viewportOffsetX)*characterScale*N64_CHARACTER_PIXEL_WIDTH, (y-viewportOffsetY)*characterScale*N64_CHARACTER_PIXEL_HEIGHT, bitmap, characterWidth, characterScale);
		return characterWidth;
	}
	return 0;
}

void N64Display::increaseCharacterScale()
{
	if (characterScale < 16)
		characterScale *= 2;
}

void N64Display::decreaseCharacterScale()
{
	if (characterScale > 1)
		characterScale /= 2;
}

void N64Display::resetViewPortOffsets()
{
	viewportOffsetX = 0;
	viewportOffsetY = 0;
}

void N64Display::updateViewPortOffsetX(int addition)
{
	viewportOffsetX += addition;
}

void N64Display::updateViewPortOffsetY(int addition)
{
	viewportOffsetY += addition;
}
