#include "core_system.h"
#include "c_rng.h"

using namespace RetroCrypto;

CoreSystem::CoreSystem()
{
	initialized = false;
	menuSystem = MenuSystem::getMenuSystem();
	inputSystem = InputSystem::getInputSystem();
	rng = static_pointer_cast<IRandomNumberGenerator>(make_shared<CRNG>());
}

CoreSystem::~CoreSystem()
{
}

CoreSystem& CoreSystem::getCoreSystem()
{
	static CoreSystem instance;
	return instance;
}

void CoreSystem::setDisplay(std::shared_ptr<IDisplay> inDisplay)
{
	if (menuSystem->getInitialized() == true)
		std::string("The display has to be set before initialization.");
	menuSystem->setDisplay(inDisplay);
}

void CoreSystem::setInputCapturer(std::shared_ptr<IInputCapturer> capturer)
{
	if (inputSystem->getInitialized() == true)
		std::string("The input capturer has to be set before initialization.");
	inputSystem->setInputCapturer(capturer);
}

void CoreSystem::setRandomNumberGenerator(std::shared_ptr<IRandomNumberGenerator> inGenerator)
{
	rng = inGenerator;
}

std::shared_ptr<IRandomNumberGenerator> CoreSystem::getRandomNumberGenerator()
{
	return rng;
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
	InputType nextInput = inputSystem->getNextInput();
	menuSystem->processInput(nextInput);
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

void CoreSystem::updateContextData(ContextUpdate update, ContextData dataContainer)
{
	context.updateData(update, dataContainer);
}

ContextData CoreSystem::getContextData()
{
	return context.getData();
}
