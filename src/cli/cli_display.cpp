#include <termios.h>

#include "cli_display.h"
#include "control_sequences.h"

CLIDisplay::CLIDisplay()
{
	ControlSequences::sendHideCursor();
	ColorSettings baseColors(TerminalColor(0, 0, 0), TerminalColor(255, 255, 255));
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
