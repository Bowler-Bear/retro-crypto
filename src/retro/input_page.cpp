#include <cstring>

#include "input_page.h"
#include "core_system.h"

using namespace RetroCrypto;

InputPage::InputPage()
: Page()
{
	description = "";
	stringSize = 1;
	charWidth = 1;
	destination = nullptr;
	usedCharSet = nullptr;
}

InputPage::InputPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: Page(inTitle, inParent)
{
	description = "";
	stringSize = 1;
	charWidth = 1;
	destination = nullptr;
	usedCharSet = nullptr;
}

void InputPage::updateSelectedOption(InputType input)
{
	if (usedCharSet == nullptr)
		return;
	switch (input)
	{
	case InputType::UP:
		{
			int32_t charSetSize = std::strlen(usedCharSet);
			int32_t startCheckIndex = inputString[selectedOptionIndex];
			uint8_t codePoints = startCheckIndex == -1 ? 0 : getCodePointCount(usedCharSet[startCheckIndex]);
			if (startCheckIndex == -1)
			{
				inputString[selectedOptionIndex] = 0;
				break;
			}
			else if (startCheckIndex >= charSetSize-1 || (startCheckIndex == charSetSize-2 && codePoints == 1) || (startCheckIndex == charSetSize-3 && codePoints == 2) || (startCheckIndex == charSetSize-4 && codePoints == 3))
			{
				inputString[selectedOptionIndex] = -1;
				break;
			}
			inputString[selectedOptionIndex] = startCheckIndex+codePoints+1;
		}
		break;
	case InputType::DOWN:
		{
			int32_t charSetSize = std::strlen(usedCharSet);
			int32_t startCheckIndex = inputString[selectedOptionIndex];
			if (startCheckIndex == -1)
			{
				startCheckIndex = charSetSize;
			}
			else if (startCheckIndex == 0)
			{
				inputString[selectedOptionIndex] = -1;
				break;
			}
			if (startCheckIndex-1 >= 0 && (usedCharSet[startCheckIndex-1] & 0x80) == 0)
			{
				inputString[selectedOptionIndex] = startCheckIndex-1;
			}
			else if(startCheckIndex-2 >= 0 && (usedCharSet[startCheckIndex-2] & 0xe0) == 0xc0)
			{
				inputString[selectedOptionIndex] = startCheckIndex-2;
			}
			else if(startCheckIndex-3 >= 0 && (usedCharSet[startCheckIndex-3] & 0xf0) == 0xe0)
			{
				inputString[selectedOptionIndex] = startCheckIndex-3;
			}
			else if(startCheckIndex-4 >= 0 && (usedCharSet[startCheckIndex-4] & 0xf8) == 0xf0)
			{
				inputString[selectedOptionIndex] = startCheckIndex-4;
			}
		}
		break;
	case InputType::LEFT:
	case InputType::RIGHT:
		if (input == InputType::RIGHT && inputString[selectedOptionIndex] == -1)
			break;
		if (input == InputType::LEFT)
		{
			if (selectedOptionIndex == 0 && inputString[inputString.size()-1] == -1)
			{
				break;
			}
			if (inputString[selectedOptionIndex] == -1)
			{
				for (size_t i = selectedOptionIndex+1; i < inputString.size(); i++)
				{
					inputString[i] = -1;
				}
			}
		}
		selectedOptionIndex = (selectedOptionIndex + (input == InputType::LEFT ? -1 : 1) + stringSize) % (stringSize);
		break;
	default:
		break;
	}
}

void InputPage::draw(shared_ptr<IDisplay> display)
{
	Page::draw(display);
	drawInput(display);
	drawDescription(display);
}

void InputPage::reset()
{
	selectedOptionIndex = 0;
	inputString.clear();
	inputString.resize(stringSize, -1);
}

void InputPage::setStringSize(uint8_t size)
{
	stringSize = size;
	inputString.resize(stringSize, -1);
}

void InputPage::setUsedCharSet(const char* charSet)
{
	usedCharSet = charSet;
}

void InputPage::setDescription(string newDescription)
{
	description = newDescription;
}

void InputPage::setDestination(shared_ptr<MenuTreeObject> newDestination)
{
	destination = newDestination;
}

void InputPage::setCharWidth(uint8_t newCharWidth)
{
	charWidth = newCharWidth;
}

shared_ptr<MenuTreeObject> InputPage::getDestination()
{
	while (selectedOptionIndex != 0 && inputString[selectedOptionIndex] == -1)
	{
		updateSelectedOption(InputType::LEFT);
	}
	if (selectedOptionIndex == 0 && inputString[selectedOptionIndex] == -1)
		return nullptr;
	return destination;
}

void InputPage::onEnter()
{
	reset();
}

void InputPage::drawInput(shared_ptr<IDisplay> display)
{
	if (usedCharSet == nullptr)
		return;
	std::string navSymbols = " ";
	std::string currentInput = " ";
	for (size_t i = 0; i < inputString.size(); i++)
	{
		uint8_t codePoints = getCodePointCount(usedCharSet[inputString[i]]);
		if (inputString[i] == -1)
			for (uint8_t j = 0; j < charWidth; j++)
				currentInput += '-';
		else
			for (uint8_t j = 0; j < codePoints+1; j++)
				currentInput += usedCharSet[inputString[i]+j];
		if (i == selectedOptionIndex)
			for (uint8_t j = 0; j < charWidth; j++)
				navSymbols += "*";
		else
			for (uint8_t j = 0; j < charWidth; j++)
				navSymbols += " ";
	}
	TextBox inputBox(currentInput);
	inputBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+8;
	inputBox.xPosition = 1;
	inputBox.width = BASE_BORDER_BOX_WIDTH-3;
	inputBox.height = 1;
	display->drawTextBox(inputBox);
	inputBox.text = navSymbols;
	inputBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+8-1;
	display->drawTextBox(inputBox);
	inputBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+8+1;
	display->drawTextBox(inputBox);
}

void InputPage::drawDescription(shared_ptr<IDisplay> display)
{
	if (description.empty())
		return;
	TextBox descriptionBox(description);
	descriptionBox.yPosition = BASE_BORDER_BOX_HEIGHT-6;
	descriptionBox.xPosition = BASE_BORDER_BOX_X_POSITION+1;
	descriptionBox.width = BASE_BORDER_BOX_WIDTH-2;
	descriptionBox.height = 5;
	descriptionBox.setBordered();
	display->drawTextBox(descriptionBox);
}

uint8_t RetroCrypto::getCodePointCount(char startCharacter)
{
	if ((startCharacter & 0x80) == 0)
	{
		return 0;
	}
	else if ((startCharacter & 0xe0) == 0xc0)
	{
		return 1;
	}
	else if ((startCharacter & 0xf0) == 0xe0)
	{
		return 2;
	}
	else if ((startCharacter & 0xf8) == 0xf0)
	{
		return 3;
	}
	return 0;
}
