#include <stdexcept>
#include <math.h>

#include "menu.h"

using namespace RetroCrypto;

Menu::Menu()
{
	title = "";
	setParent(nullptr);
	setShouldReparent(MENU_TREE_OBJECT_REPARENT_DEFAULT);
	selectedOptionIndex = 0;
	setForwardAction(nullptr);
	setBackwardAction(nullptr);
}

Menu::Menu(std::string inTitle, std::shared_ptr<MenuTreeObject> inParent)
: Menu()
{
	title = inTitle;
	setParent(inParent);
}

void Menu::setTitle(std::string title)
{
	this->title = title;
}

std::string Menu::getTitle()
{
	return title;
}

void Menu::drawTitle(std::shared_ptr<IDisplay> display)
{
	TextBox titleBox(title);
	titleBox.yPosition = MENU_TITLE_BOX_Y_POSITION;
	titleBox.xPosition = (MENU_BOX_WIDTH-titleBox.text.size())/2;
	titleBox.width = titleBox.text.size()+2;
	titleBox.height = MENU_TITLE_BOX_HEIGHT;
	titleBox.setUnderlined();
	titleBox.setBold();
	display->drawTextBox(titleBox);
}

void Menu::drawOptions(std::shared_ptr<IDisplay> display)
{
	const uint8_t optionCount = getOptionCount();
	const uint8_t startIndex = min(max(0, (int)selectedOptionIndex-MENU_MAX_OPTIONS_SHOWN/2), max(0, (int)optionCount-MENU_MAX_OPTIONS_SHOWN));
	const uint8_t endIndex = min(startIndex + min(MENU_MAX_OPTIONS_SHOWN, (int)optionCount), (int)optionCount);
	for(uint8_t i = startIndex; i < endIndex; i++)
	{
		const shared_ptr<MenuOption> option = options[i];
		TextBox optionBox("");
		if (i == selectedOptionIndex)
			optionBox.text = "-> "+option->getLabel()+" <-";
		else if (i == startIndex && startIndex != 0)
			optionBox.text = "---Previous---";
		else if (i == endIndex-1 && endIndex != optionCount)
			optionBox.text = "---Next---";
		else
			optionBox.text = option->getLabel();
		optionBox.yPosition = (MENU_TITLE_BOX_Y_POSITION+3)+2*(i-startIndex);
		optionBox.xPosition = (MENU_BOX_WIDTH-optionBox.text.size())/2;
		optionBox.width = optionBox.text.size()+3;
		optionBox.height = 3;
		if (i == selectedOptionIndex)
		{
			optionBox.setBordered();
			optionBox.setBold();
			optionBox.setBlinking();
		}
		display->drawTextBox(optionBox);
	}
}

void Menu::drawDescription(std::shared_ptr<IDisplay> display)
{
	if (selectedOptionIndex < getOptionCount())
	{
		TextBox descriptionBox(options[selectedOptionIndex]->getDescription());
		descriptionBox.yPosition = MENU_BOX_HEIGHT-6;
		descriptionBox.xPosition = 2;
		descriptionBox.width = MENU_BOX_WIDTH-3;
		descriptionBox.height = 5;
		descriptionBox.setBordered();
		display->drawTextBox(descriptionBox);
	}
}

void Menu::draw(std::shared_ptr<IDisplay> display)
{
	drawBorder(display);
	drawTitle(display);
	drawOptions(display);
	drawDescription(display);
}

void Menu::updateSelectedOption(InputType input)
{
	if (input != InputType::UP && input != InputType::DOWN)
		return;
	const uint8_t optionCount = getOptionCount();
	if (optionCount <= 0)
	{
		selectedOptionIndex = 0;
		return;
	}
	int8_t addition = input == InputType::UP ? -1 : 1;
	uint8_t newOptionIndex = (selectedOptionIndex + addition + optionCount) % optionCount;
	do
	{
		if (!options[newOptionIndex]->getDisabled())
		{
			selectedOptionIndex = newOptionIndex;
			return;
		}
		newOptionIndex = (selectedOptionIndex + addition + optionCount) % optionCount;
	} while(newOptionIndex != selectedOptionIndex);
	selectedOptionIndex = newOptionIndex;
}

std::shared_ptr<MenuTreeObject> Menu::getDestination()
{
	if (selectedOptionIndex < 0 || selectedOptionIndex >= getOptionCount())
		throw std::runtime_error(std::string(__func__)+": Error trying to navigate to invalid option index");
	std::shared_ptr<MenuOption> selectedOption = options[selectedOptionIndex];
	if (selectedOption == nullptr)
		throw std::runtime_error(std::string(__func__)+": Selected option is a null pointer");
	if (selectedOption->getDisabled())
		throw std::runtime_error(std::string(__func__)+": Selected option was disabled and shouldn't have been selected");
	return selectedOption->getDestination();
}

void Menu::onForward()
{
	MenuTreeObject::onForward();
	if (selectedOptionIndex < 0 || selectedOptionIndex >= getOptionCount())
		return;
	std::shared_ptr<MenuOption> selectedOption = options[selectedOptionIndex];
	if (selectedOption != nullptr && !selectedOption->getDisabled())
		selectedOption->onSelected();
}
