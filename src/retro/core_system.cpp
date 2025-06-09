#include "core_system.h"

using namespace RetroCrypto;

CoreSystem::CoreSystem()
{
	initialized = false;
	menuSystem = MenuSystem::getMenuSystem();
	inputSystem = InputSystem::getInputSystem();
}

CoreSystem::~CoreSystem()
{
}

CoreSystem& CoreSystem::getCoreSystem()
{
	static CoreSystem instance;
	return instance;
}

void CoreSystem::setDisplayFactory(IDisplayFactory& factory)
{
	if (menuSystem->getInitialized() == true)
		std::string("The display factory has to be set before initialization.");
	menuSystem->setDisplayFactory(factory);
}

void CoreSystem::tick()
{
	if (getInitialized() == false)
		throw std::string("CoreSystem must be initialized before running.");
	if (getQuitRequested() == true)
		return;
	if (inputSystem->getQuitRequested() != getQuitRequested())
		setQuitRequested(inputSystem->getQuitRequested());
	if (menuSystem->getQuitRequested() != getQuitRequested())
		setQuitRequested(menuSystem->getQuitRequested());
	inputSystem->tick();
	menuSystem->tick();
}

bool CoreSystem::init()
{
	if (getInitialized() == true)
		return getInitialized();
	if (!menuSystem->init())
		throw std::string("MenuSystem failed to be initialized.");
	if (!inputSystem->init())
		throw std::string("InputSystem failed to be initialized.");
	setInitialized(true);
	return getInitialized();
}
