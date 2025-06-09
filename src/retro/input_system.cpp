#include <string>

#include "input_system.h"

using namespace RetroCrypto;

InputSystem::InputSystem()
{
	initialized = false;
	inputCapturer = nullptr;
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
	if (inputCapturer == nullptr)
		throw std::string("No input capturer in InputSystem.");
	inputQueue.push(inputCapturer->getInput());
}

bool InputSystem::init()
{
	if (getInitialized() == true)
		return getInitialized();
	if (inputCapturer == nullptr)
		throw std::string("Input capturer not passed to InputSystem.");
	setInitialized(true);
	return getInitialized();
}

void InputSystem::setInputCapturer(std::shared_ptr<IInputCapturer> capturer)
{
	inputCapturer = capturer;
}

InputType InputSystem::getNextInput()
{
	InputType nextInput = inputQueue.front();
	inputQueue.pop();
	return nextInput;
}
