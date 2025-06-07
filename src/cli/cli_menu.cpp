#include <iostream>

#include "cli_menu.h"

CLIMenu::CLIMenu() : IMenu()
{
}

void CLIMenu::redraw()
{
	#define MENU_BORDER_PADDING 1
	#define MENU_HEIGHT 25
	#define MENU_WIDTH 100
	const char borderChar = '*';
	std::string title = "CLI Menu";
	for (int y = MENU_BORDER_PADDING+1; y < MENU_HEIGHT+MENU_BORDER_PADDING+1; y++)
	{
		if (y <= MENU_BORDER_PADDING || y >= MENU_HEIGHT+1)
			continue;
		if (y == MENU_BORDER_PADDING+1 || y == MENU_HEIGHT+1-MENU_BORDER_PADDING)
		{
			for (int x = MENU_BORDER_PADDING+1; x < MENU_WIDTH+1; x++)
			{
				if (x <= MENU_BORDER_PADDING || x >= MENU_WIDTH-MENU_BORDER_PADDING)
					continue;
				std::cout << "\x1b[" << y << ";" << x << "H";
				std::cout << borderChar;
			}
			continue;
		}
		if (y == MENU_BORDER_PADDING+2)
		{
			std::cout << "\x1b[" << y << ";" << (MENU_WIDTH/2)-8/2 << "H";
			std::cout << title;
		}
		std::cout << "\x1b[" << y << ";" << MENU_BORDER_PADDING+1 << "H";
		std::cout << borderChar;
		std::cout << "\x1b[" << y << ";" << MENU_WIDTH-(MENU_BORDER_PADDING+1) << "H";
		std::cout << borderChar;
	}
	std::flush(std::cout);
}

void CLIMenu::clear()
{
	std::cout << "\x1b[2J";
	std::cout << "\x1b[3J";
	std::cout << "\x1b[0;0H";
	std::flush(std::cout);
}
