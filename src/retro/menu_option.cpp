#include "menu_option.h"
#include "menu.h"

using namespace RetroCrypto;

MenuOption::MenuOption(const std::shared_ptr<Menu> menu, const std::string& label, const std::string& description)
: label(label), description(description)
{
	parent = menu;
	menu->add(make_shared<MenuOption>(*this));
}

void MenuOption::add(std::shared_ptr<MenuTreeObject> destination)
{
	this->destination = destination;
}
