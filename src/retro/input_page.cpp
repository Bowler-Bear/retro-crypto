#include <cstring>

#include "input_page.h"
#include "core_system.h"

using namespace RetroCrypto;

InputPage::InputPage()
: Page()
{
	description = "";
	stringSize = 1;
	destination = nullptr;
	usedCharSet = nullptr;
}

InputPage::InputPage(string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: Page(inTitle, inParent)
{
	description = "";
	stringSize = 1;
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
	case InputType::DOWN:
		{
			int32_t charSetSize = std::strlen(usedCharSet);
			if (inputString[selectedOptionIndex] == -1)
			{
				inputString[selectedOptionIndex] = input == InputType::DOWN ? charSetSize-1 : 0;
				break;
			}
			int32_t nextValue = (inputString[selectedOptionIndex] + charSetSize + (input == InputType::DOWN ? -1 : 1)) % charSetSize;
			if ((inputString[selectedOptionIndex] == 0 && nextValue == charSetSize-1) || (inputString[selectedOptionIndex] == charSetSize-1 && nextValue == 0))
				nextValue = -1;
			inputString[selectedOptionIndex] = nextValue;
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
void InputPage::setStringSize(uint8_t size)
{
	stringSize = size;
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
	selectedOptionIndex = 0;
	inputString.clear();
	inputString.resize(stringSize, -1);
}

void InputPage::onForward()
{
	//set global context
}

void InputPage::drawInput(shared_ptr<IDisplay> display)
{
	if (usedCharSet == nullptr)
		return;
	for(size_t i = 0; i < inputString.size(); i++)
	{
		int32_t charIndex = inputString[i];
		TextBox inputBox;
		inputBox.text = string(1, charIndex == -1 ? '-' : usedCharSet[charIndex]);
		inputBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+8;
		inputBox.xPosition = (BASE_BORDER_BOX_WIDTH-stringSize)/2+i;
		inputBox.width = 1;
		inputBox.height = 1;
		if (i == selectedOptionIndex)
		{
			inputBox.setBold();
			TextBox asteriskBox("*");
			asteriskBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+8-1;
			asteriskBox.xPosition = (BASE_BORDER_BOX_WIDTH-stringSize)/2+i;
			asteriskBox.width = 1;
			asteriskBox.height = 1;
			display->drawTextBox(asteriskBox);
			asteriskBox.yPosition = PAGE_TITLE_BOX_Y_POSITION+8+1;
			display->drawTextBox(asteriskBox);
		}
		display->drawTextBox(inputBox);
	}
}

void InputPage::drawDescription(shared_ptr<IDisplay> display)
{
	if (description.empty())
		return;
	TextBox descriptionBox(description);
	descriptionBox.yPosition = BASE_BORDER_BOX_HEIGHT-6;
	descriptionBox.xPosition = 2;
	descriptionBox.width = BASE_BORDER_BOX_WIDTH-3;
	descriptionBox.height = 5;
	descriptionBox.setBordered();
	display->drawTextBox(descriptionBox);
}
