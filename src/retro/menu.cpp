#include "menu.h"

using namespace RetroCrypto;

Menu::Menu()
{
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

void Menu::draw(std::shared_ptr<IDisplay> display, int selectedOptionIndex)
{
	Box border;
	border.xPosition = 0;
	border.yPosition = 0;
	border.width = 100;
	border.height = 25;
	display->drawBox(border);
	TextBox titleBox(title);
	titleBox.yPosition = 3;
	titleBox.xPosition = (border.width-titleBox.text.size())/2;
	titleBox.width = titleBox.text.size()+2;
	titleBox.height = 1+2;
	titleBox.setUnderlined();
	titleBox.setBold();
	display->drawTextBox(titleBox);
	for(int i = 0; i < options.size(); i++)
	{
		const shared_ptr<MenuOption> option = options[i];
		TextBox optionBox(i == selectedOptionIndex?"-> "+option->getLabel()+" <-":option->getLabel());
		optionBox.yPosition = (titleBox.yPosition+3)+2*i;
		optionBox.xPosition = (border.width-optionBox.text.size())/2;
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
	if (selectedOptionIndex < options.size())
	{
		TextBox descriptionBox(options[optionIndex]->getDescription());
		descriptionBox.yPosition = border.height-6;
		descriptionBox.xPosition = 2;
		descriptionBox.width = border.width-3;
		descriptionBox.height = 5;
		descriptionBox.setBordered();
		display->drawTextBox(descriptionBox);
	}
}
