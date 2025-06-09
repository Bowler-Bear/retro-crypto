#include "menu_system.h"

using namespace RetroCrypto;

MenuSystem::MenuSystem()
{
	initialized = false;
	quitRequested = false;
	menuTree = nullptr;
	currentMenuPosition = nullptr;
	optionIndex = 0;
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
	if (menuTree == nullptr)
		menuTree = make_shared<MenuTree>();
	menuTree->init();
	currentMenuPosition = menuTree->getRoot();

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
	currentMenuPosition->draw(display, optionIndex);
}

void MenuSystem::processInput(InputType input)
{
	switch (input)
	{
	case InputType::UP:
	case InputType::DOWN:
		{
			std::vector<std::shared_ptr<MenuOption>>& options = currentMenuPosition->getOptions();
			int addition = input == InputType::UP ? -1 : 1;
			int newOptionIndex = optionIndex + addition;
			do
			{
				if (newOptionIndex < 0)
					newOptionIndex = options.size()-1;
				if (newOptionIndex >= options.size())
					newOptionIndex = 0;
				if (!options[newOptionIndex]->getDisabled())
				{
					optionIndex = newOptionIndex;
					break;
				}
				newOptionIndex += addition;
			} while(newOptionIndex != optionIndex);
		}
		break;
	case InputType::LEFT:
	case InputType::RIGHT:
	case InputType::FORWARD:
	case InputType::BACK:
	case InputType::NONE:
	default:
		break;
	}
}
