#include "menu.h"

using namespace RetroCrypto;

Menu::Menu()
{
	title = "";
	setParent(nullptr);
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
