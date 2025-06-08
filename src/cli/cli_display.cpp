#include <iostream>

#include "cli_display.h"
#include "control_sequences.h"

CLIDisplay::CLIDisplay() : IDisplay()
{
}

void CLIDisplay::redraw()
{
	#define MENU_BORDER_PADDING 1
	#define MENU_HEIGHT 25
	#define MENU_WIDTH 100
	std::string borderChar("*");
	std::string title("CLI Menu");
	ColorSettings baseColors(TerminalColor(0, 0, 0), TerminalColor(255, 255, 255));
	ControlSequences::sendRestoreColorSettings(baseColors);
	for (int y = MENU_BORDER_PADDING+1; y < MENU_HEIGHT+MENU_BORDER_PADDING+1; y++)
	{
		if (y <= MENU_BORDER_PADDING || y >= MENU_HEIGHT+2)
			continue;
		if (y == MENU_BORDER_PADDING+1 || y == MENU_HEIGHT+1)
		{
			for (int x = MENU_BORDER_PADDING+1; x < MENU_WIDTH+1; x++)
			{
				if (x <= MENU_BORDER_PADDING || x >= MENU_WIDTH-MENU_BORDER_PADDING)
					continue;
				ControlSequences::sendSetCursorPostion(y, x);
				ControlSequences::sendText(borderChar);
			}
			continue;
		}
		if (y == MENU_BORDER_PADDING+2)
		{
			ControlSequences::sendSetCursorPostion(y, (MENU_WIDTH/2)-title.size()/2);
			ControlSequences::sendText(title, TextProperties::TEXT_BOLD | TextProperties::TEXT_UNDERLINED | TextProperties::TEXT_OVERLINED);
		}
		if (y > MENU_BORDER_PADDING+9 && y < MENU_BORDER_PADDING+14)
		{
			string option = "Menu Option(";
			option += to_string(y);
			option += ")";
			ControlSequences::sendSetCursorPostion(y, (MENU_WIDTH/2)-option.size()/2);
			ControlSequences::sendText(option, y == MENU_BORDER_PADDING+12?TextProperties::TEXT_BLINK:0);
		}
		ControlSequences::sendSetCursorPostion(y, MENU_BORDER_PADDING+1);
		ControlSequences::sendText(borderChar);
		ControlSequences::sendSetCursorPostion(y, MENU_WIDTH-(MENU_BORDER_PADDING+1));
		ControlSequences::sendText(borderChar);
	}
	ControlSequences::flush();
}

void CLIDisplay::clear()
{
	ControlSequences::sendClearDisplay();
	ControlSequences::sendSetCursorPostion(1,1);
	ControlSequences::flush();
}
