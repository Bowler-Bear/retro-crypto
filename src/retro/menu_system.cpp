#include "menu_system.h"

using namespace RetroCrypto;

MenuSystem::MenuSystem()
{
	quitRequested = false;
	menuFactory = nullptr;
	menu = nullptr;
}

MenuSystem::~MenuSystem()
{
	if (menu != nullptr)
		delete menu;
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

void MenuSystem::setMenuFactory(IMenuFactory& factory)
{
	menuFactory = &factory;
}

void MenuSystem::init()
{
	if (menuFactory != nullptr && menu == nullptr)
	{
		menu = menuFactory->constructMenu();
	}
}

void MenuSystem::redraw()
{
	if (menu != nullptr)
	{
		menu->clear();
		menu->redraw();
	}
}
