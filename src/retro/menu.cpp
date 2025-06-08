#include "menu.h"

using namespace RetroCrypto;

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
