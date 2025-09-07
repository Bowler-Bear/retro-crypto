#include <termios.h>

#include "cli_display.h"
#include "control_sequences.h"

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

CLIDisplay::CLIDisplay()
{
	ControlSequences::sendHideCursor();
	ColorSettings baseColors(TerminalColor(BG_COLOR_RED, BG_COLOR_GREEN, BG_COLOR_BLUE), TerminalColor(FG_COLOR_RED, FG_COLOR_GREEN, FG_COLOR_BLUE));
	ControlSequences::sendRestoreColorSettings(baseColors);
	termios attributes = {0};
	tcgetattr(0, &attributes);
	attributes.c_lflag &= ~ICANON;
	attributes.c_lflag &= ~ECHO;
	attributes.c_cc[VMIN] = 0;
	attributes.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &attributes);
}

CLIDisplay::~CLIDisplay()
{
	ControlSequences::sendShowCursor();
}

void CLIDisplay::redraw()
{
}

void CLIDisplay::clear()
{
	ControlSequences::sendClearDisplay();
	ControlSequences::sendSetCursorPostion(1,1);
	ControlSequences::flush();
}

void CLIDisplay::drawBox(const Box& box)
{
	std::string borderChar("*");
	for (int y = box.yPosition+1; y < box.yPosition+box.height+1; y++)
	{
		if (y == box.yPosition+1 || y == box.yPosition+box.height)
		{
			for (int x = box.xPosition; x < box.xPosition+box.width+1; x++)
			{
				ControlSequences::sendSetCursorPostion(y, x);
				ControlSequences::sendText(borderChar);
			}
			continue;
		}
		ControlSequences::sendSetCursorPostion(y, box.xPosition);
		ControlSequences::sendText(borderChar);
		ControlSequences::sendSetCursorPostion(y, box.xPosition+box.width);
		ControlSequences::sendText(borderChar);
	}
	ControlSequences::flush();
}

void CLIDisplay::drawTextBox(const TextBox& textBox)
{
	if (textBox.isBlinking())
		ControlSequences::sendSetSlowBlink();
	if (textBox.isBordered())
		drawBox(textBox);
	ControlSequences::sendSetCursorPostion(textBox.yPosition+1+textBox.height/2, textBox.xPosition+1+(textBox.width-textBox.text.size())/2);
	ControlSequences::sendText(textBox);
	ControlSequences::flush();
}

void CLIDisplay::drawQrBox(const QrBox& qrBox)
{
	for (int y = 0; y < qrBox.height; y++)
	{
		for (int x = 0; x < qrBox.width; x++)
		{
			TextBox blockBox(qrBox.qrCode.getModule(x, y) ? "#" : " ");
			blockBox.yPosition = qrBox.yPosition+y;
			blockBox.xPosition = qrBox.xPosition+x;
			blockBox.width = 1;
			blockBox.height = 1;
			drawTextBox(blockBox);
		}
	}
}
