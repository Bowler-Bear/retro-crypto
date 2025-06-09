#include <unistd.h>
#include <iostream>

#include "cli_capturer.h"

#define ENTER_KEY 10
#define ESC_KEY 27
#define A_KEY 65
#define B_KEY 66
#define C_KEY 67
#define D_KEY 68
#define S_KEY 83
#define W_KEY 87
#define OPEN_BRACKET_KEY 91
#define a_KEY 97
#define d_KEY 100
#define s_KEY 115
#define w_KEY 119
#define BACKSPACE_KEY 127

void CLICapturer::checkInputThroughCIN()
{
	while(true)
	{
		char readChar = '\0';
		bool escapeSequence = false;
		read(0, &readChar, 1);
		if (readChar == ESC_KEY)
		{
			if (read(0, &readChar, 1) && readChar == OPEN_BRACKET_KEY)
			{
				escapeSequence = true;
				read(0, &readChar, 1);
			}
		}
		if (readChar != 0)
		{
			switch (readChar)
			{
			case ENTER_KEY:
				lastInput = InputType::FORWARD;
				break;
			case A_KEY:
				lastInput = escapeSequence ? InputType::UP : InputType::LEFT;
				break;
			case B_KEY:
				if (escapeSequence)
					lastInput = InputType::DOWN;
				break;
			case C_KEY:
				if (escapeSequence)
					lastInput = InputType::RIGHT;
				break;
			case D_KEY:
				if (escapeSequence)
					lastInput = escapeSequence ? InputType::LEFT : InputType::RIGHT;
				break;
			case a_KEY:
				lastInput = InputType::LEFT;
				break;
			case d_KEY:
				lastInput = InputType::RIGHT;
				break;
			case W_KEY:
			case w_KEY:
				lastInput = InputType::UP;
				break;
			case S_KEY:
			case s_KEY:
				lastInput = InputType::DOWN;
				break;
			case BACKSPACE_KEY:
				lastInput = InputType::BACK;
				break;
			default:
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

CLICapturer::CLICapturer()
{
	lastInput = InputType::NONE;
	inputCheckingThread = std::thread(&CLICapturer::checkInputThroughCIN, this);
}

InputType CLICapturer::getInput()
{
	InputType returnedInput = lastInput;
	lastInput = InputType::NONE;
	return returnedInput;
}
