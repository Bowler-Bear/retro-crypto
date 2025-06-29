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
			std::shared_ptr<MenuTreeObject> destination = currentMenuPosition->getDestination(selectedOptionIndex);
			if (destination == nullptr)
				throw std::string("Missing destination for menu option.");
			destination->setParent(currentMenuPosition);
			currentMenuPosition->onForward(selectedOptionIndex);
			currentMenuPosition = destination;
			selectedOptionIndex = 0;
			destination->onEnter();
		}
		break;
	case InputType::BACK:
		{
			std::shared_ptr<MenuTreeObject> parent = currentMenuPosition->getParent();
			if (parent == nullptr)
				break;
			currentMenuPosition->onBackward(selectedOptionIndex);
			currentMenuPosition = parent;
			selectedOptionIndex = 0;
			parent->onEnter();
		}
		break;
	case InputType::NONE:
	default:
		break;
	}
}
