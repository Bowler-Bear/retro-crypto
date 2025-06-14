#include "menu.h"

using namespace RetroCrypto;

Menu::Menu()
{
	title = "";
	setParent(nullptr);
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

void Menu::add(std::shared_ptr<MenuTreeObject> option)
{
	add(std::dynamic_pointer_cast<MenuOption>(option));
}

void Menu::add(std::shared_ptr<MenuOption> option)
{
	options.push_back(option);
}

void Menu::drawTitle(std::shared_ptr<IDisplay> display)
{
	Box border;
	border.xPosition = MENU_BOX_X_POSITION;
	border.yPosition = MENU_BOX_Y_POSITION;
	border.width = MENU_BOX_WIDTH;
	border.height = MENU_BOX_HEIGHT;
	display->drawBox(border);
	TextBox titleBox(title);
	titleBox.yPosition = MENU_TITLE_BOX_Y_POSITION;
	titleBox.xPosition = (border.width-titleBox.text.size())/2;
	titleBox.width = titleBox.text.size()+2;
	titleBox.height = MENU_TITLE_BOX_HEIGHT;
	titleBox.setUnderlined();
	titleBox.setBold();
	display->drawTextBox(titleBox);
}

void Menu::drawOptions(std::shared_ptr<IDisplay> display, int selectedOptionIndex)
{
	for(int i = 0; i < options.size(); i++)
	{
		const shared_ptr<MenuOption> option = options[i];
		TextBox optionBox(i == selectedOptionIndex?"-> "+option->getLabel()+" <-":option->getLabel());
		optionBox.yPosition = (MENU_TITLE_BOX_Y_POSITION+3)+2*i;
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

void Menu::drawDescription(std::shared_ptr<IDisplay> display, int selectedOptionIndex)
{
	if (selectedOptionIndex < options.size())
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

void Menu::draw(std::shared_ptr<IDisplay> display, int selectedOptionIndex)
{
	drawTitle(display);
	drawOptions(display, selectedOptionIndex);
	drawDescription(display, selectedOptionIndex);
}

int Menu::getNewSelectedOption(int selectedOptionIndex, InputType input)
{
	if (options.size() <= 0)
		return 0;
	int addition = input == InputType::UP ? -1 : 1;
	int newOptionIndex = selectedOptionIndex + addition;
	do
	{
		if (newOptionIndex < 0)
			newOptionIndex = options.size()-1;
		if (newOptionIndex >= options.size())
			newOptionIndex = 0;
		if (!options[newOptionIndex]->getDisabled())
		{
			return newOptionIndex;
		}
		newOptionIndex += addition;
	} while(newOptionIndex != selectedOptionIndex);
	return newOptionIndex;
}

std::shared_ptr<MenuTreeObject> Menu::getDestination(int selectedOptionIndex)
{
	if (selectedOptionIndex < 0 || selectedOptionIndex >= options.size())
		throw std::string("Error trying to navigate to invalid option index");
	std::shared_ptr<MenuTreeObject> destination = options[selectedOptionIndex]->getDestination();
	if (destination == nullptr)
		throw std::string("Missing destination for menu option ")+options[selectedOptionIndex]->getLabel();
	return destination;
}

void Menu::onForward(int selectedOptionIndex)
{
	MenuTreeObject::onForward(selectedOptionIndex);
	if (selectedOptionIndex < 0 || selectedOptionIndex >= options.size())
		return;
	std::shared_ptr<MenuOption> selectedOption = options[selectedOptionIndex];
	if (selectedOption != nullptr && !selectedOption->getDisabled())
		selectedOption->onSelected();
}
