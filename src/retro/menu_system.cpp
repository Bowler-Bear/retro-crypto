#include "menu_option.h"
#include "menu_system.h"

using namespace RetroCrypto;

MenuSystem::MenuSystem()
{
	initialized = false;
	quitRequested = false;
	menuTree = MenuTree();
	currentMenuPosition = menuTree.getRoot();
	selectedOptionIndex = 0;
	display = nullptr;
}

MenuSystem::~MenuSystem()
{
}

MenuSystem* MenuSystem::getMenuSystem()
{
	static MenuSystem instance;
	return &instance;
}

void MenuSystem::setDisplay(std::shared_ptr<IDisplay> inDisplay)
{
	display = inDisplay;
}

bool MenuSystem::init()
{
	if (getInitialized() == true)
		return getInitialized();
	if (display == nullptr)
		throw std::string("No IDisplay passed to MenuSystem.");
	setInitialized(true);
	return getInitialized();
}

void MenuSystem::tick()
{
	if (getInitialized() == false)
		throw std::string("MenuSystem must be initialized before running.");
	redraw();
}

void MenuSystem::redraw()
{
	if (display == nullptr)
		throw std::string("MenuSystem has no display to draw to.");
	display->clear();
	currentMenuPosition->draw(display, selectedOptionIndex);
}

void MenuSystem::processInput(InputType input)
{
	switch (input)
	{
	case InputType::UP:
	case InputType::DOWN:
	case InputType::LEFT:
	case InputType::RIGHT:
		selectedOptionIndex = currentMenuPosition->getNewSelectedOption(selectedOptionIndex, input);
		break;
	case InputType::FORWARD:
		{
			std::vector<std::shared_ptr<MenuOption>>& options = currentMenuPosition->getOptions();
			if (selectedOptionIndex < 0 || selectedOptionIndex >= options.size())
				throw std::string("Error trying to navigate to invalid option index");
			std::shared_ptr<MenuTreeObject> newDestination = options[selectedOptionIndex]->getDestination();
			if (newDestination == nullptr)
				throw std::string("Missing destination for menu option ")+options[selectedOptionIndex]->getLabel();
			currentMenuPosition = newDestination;
			selectedOptionIndex = 0;
		}
		break;
	case InputType::BACK:
		{
			std::shared_ptr<MenuTreeObject> parent = currentMenuPosition->getParent();
			if (parent == nullptr)
				break;
			currentMenuPosition = parent;
			selectedOptionIndex = 0;
		}
		break;
	case InputType::NONE:
	default:
		break;
	}
}
