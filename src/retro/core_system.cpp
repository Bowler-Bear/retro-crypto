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
		throw std::runtime_error(std::string(__func__)+": The display has to be set before initialization.");
	menuSystem->setDisplay(inDisplay);
}

void CoreSystem::setInputCapturer(std::shared_ptr<IInputCapturer> capturer)
{
	if (inputSystem->getInitialized() == true)
		throw std::runtime_error(std::string(__func__)+": The input capturer has to be set before initialization.");
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
		throw std::runtime_error(std::string(__func__)+": CoreSystem must be initialized before running.");
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
		throw std::runtime_error(std::string(__func__)+": MenuSystem failed to be initialized.");
	if (!inputSystem->init())
		throw std::runtime_error(std::string(__func__)+": InputSystem failed to be initialized.");
	setInitialized(true);
	return getInitialized();
}

void CoreSystem::updateContextData(ContextUpdate update, ContextData dataContainer)
{
	if (update & ContextUpdate::SEED_SIZE && dataContainer.seedSize <= MAXIMUM_SEED_SIZE)
	{
		contextData.seedSize = dataContainer.seedSize;
	}
	if (update & ContextUpdate::SEED)
	{
		contextData.setSeed(dataContainer.seed, dataContainer.seedSize);
	}
	if (update & ContextUpdate::MNEMONIC_TYPE)
	{
		contextData.mnemonicType = dataContainer.mnemonicType;
	}
	if (update & ContextUpdate::MNEMONIC)
	{
		contextData.mnemonic = dataContainer.mnemonic;
	}
	if (update & ContextUpdate::CRYPTO)
	{
		contextData.crypto = dataContainer.crypto;
	}
}

ContextData CoreSystem::getContextData()
{
	return contextData;
}
