#include "menu_system.h"

using namespace RetroCrypto;

MenuSystem::MenuSystem()
{
	quitRequested = false;
	menuTree = nullptr;
	currentMenuPosition = nullptr;
	optionIndex = 0;
	displayFactory = nullptr;
	display = nullptr;
}

MenuSystem::~MenuSystem()
{
	if (display != nullptr)
		delete display;
}

MenuSystem* MenuSystem::getMenuSystem()
{
	static MenuSystem instance;
	return &instance;
}

bool MenuSystem::getQuitRequested()
{
	return quitRequested;
}

void MenuSystem::setQuitRequested(bool newValue)
{
	quitRequested = newValue;
}

void MenuSystem::setDisplayFactory(IDisplayFactory& factory)
{
	displayFactory = &factory;
}

void MenuSystem::init()
{
	if (menuTree == nullptr)
		menuTree = make_shared<MenuTree>();
	menuTree->init();
	currentMenuPosition = menuTree->getRoot();

	if (displayFactory != nullptr && display == nullptr)
	{
		display = displayFactory->constructDisplay();
	}
}

void MenuSystem::redraw()
{
	if (display != nullptr)
	{
		display->clear();
		currentMenuPosition->draw(display, 1);
	}
}
