#include <stdexcept>

#include "menu_system.h"

using namespace RetroCrypto;

MenuSystem::MenuSystem()
{
	initialized = false;
	quitRequested = false;
	menuTree = MenuTree();
	currentMenuPosition = menuTree.getRoot();
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
		throw std::runtime_error(std::string(__func__)+": No IDisplay passed to MenuSystem.");
	setInitialized(true);
	return getInitialized();
}

void MenuSystem::tick()
{
	if (getInitialized() == false)
		throw std::runtime_error(std::string(__func__)+": MenuSystem must be initialized before running.");
	currentMenuPosition->tick();
	redraw();
}

void MenuSystem::redraw()
{
	if (display == nullptr)
		throw std::runtime_error(std::string(__func__)+": MenuSystem has no display to draw to.");
	display->clear();
	currentMenuPosition->draw(display);
}

void MenuSystem::processInput(InputType input)
{
	switch (input)
	{
	case InputType::UP:
	case InputType::DOWN:
	case InputType::LEFT:
	case InputType::RIGHT:
		currentMenuPosition->updateSelectedOption(input);
		break;
	case InputType::FORWARD:
		{
			if (currentMenuPosition->canConsumeAllInputs() && currentMenuPosition->consumeInput(input))
					return;
			std::shared_ptr<MenuTreeObject> destination = currentMenuPosition->getDestination();
			if (destination == nullptr)
				return;
			if (destination->getShouldReparent())
				destination->setParent(currentMenuPosition);
			currentMenuPosition->onForward();
			currentMenuPosition = destination;
			destination->onEnter();
		}
		break;
	case InputType::BACK:
		{
			if (currentMenuPosition->canConsumeAllInputs() && currentMenuPosition->consumeInput(input))
					return;
			std::shared_ptr<MenuTreeObject> parent = currentMenuPosition->getParent();
			if (parent == nullptr)
				break;
			currentMenuPosition->onBackward();
			currentMenuPosition = parent;
			parent->onEnter();
		}
		break;
	case InputType::NONE:
	default:
		break;
	}
}

void MenuSystem::setMenuTree(MenuTree& newMenuTree)
{
	menuTree = newMenuTree;
	currentMenuPosition = menuTree.getRoot();
}
