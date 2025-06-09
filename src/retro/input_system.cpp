#include <string>

#include "input_system.h"

using namespace RetroCrypto;

InputSystem::InputSystem()
{
	initialized = false;
}

InputSystem::~InputSystem()
{
}

InputSystem* InputSystem::getInputSystem()
{
	static InputSystem instance;
	return &instance;
}

void InputSystem::tick()
{
	if (getInitialized() == false)
		throw std::string("InputSystem must be initialized before running.");
	if (getQuitRequested() == true)
		return;
}

bool InputSystem::init()
{
	if (getInitialized() == true)
		return getInitialized();
	setInitialized(true);
	return getInitialized();
}
