#include "menu_system.h"

using namespace RetroCrypto;

MenuSystem::MenuSystem()
{
	initialized = false;
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

void MenuSystem::setDisplayFactory(IDisplayFactory& factory)
{
	displayFactory = &factory;
}

bool MenuSystem::init()
{
	if (getInitialized() == true)
		return getInitialized();
	if (menuTree == nullptr)
		menuTree = make_shared<MenuTree>();
	menuTree->init();
	currentMenuPosition = menuTree->getRoot();

	if (displayFactory == nullptr)
		throw std::string("No IDisplayFactcor passed to MenuSystem.");
	if (display == nullptr)
		display = displayFactory->constructDisplay();
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
	currentMenuPosition->draw(display, 1);
}
