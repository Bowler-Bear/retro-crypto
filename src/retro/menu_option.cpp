#include "menu_option.h"
#include "menu.h"

using namespace RetroCrypto;

MenuOption::MenuOption(const std::shared_ptr<Menu> menu, const std::string& label, const std::string& description)
: label(label), description(description)
{
	parent = menu;
	onSelectedFunction = nullptr;
}

void MenuOption::setDestination(std::shared_ptr<MenuTreeObject> destination)
{
	this->destination = destination;
}

shared_ptr<MenuTreeObject> MenuOption::getDestination()
{
	return destination;
}

string MenuOption::getLabel()
{
	return label;
}

string MenuOption::getDescription()
{
	return description;
}

bool MenuOption::getDisabled()
{
	return disabled;
}

void MenuOption::setDisabled(bool newValue)
{
	disabled = newValue;
}

void MenuOption::setOnSelectedFunction(BlockingFunction function)
{
	onSelectedFunction = function;
}

void MenuOption::onSelected()
{
	if (onSelectedFunction != nullptr)
		onSelectedFunction();
}
