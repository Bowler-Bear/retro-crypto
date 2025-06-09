#ifndef CLI_CAPTURER_H
#define CLI_CAPTURER_H

#include <thread>

#include "input_capturer.h"

using namespace RetroCrypto;

class CLICapturer: public IInputCapturer
{
private:
	InputType lastInput;
	std::thread inputCheckingThread;
public:
	CLICapturer();
	void checkInputThroughCIN();
	virtual InputType getInput() override;
};
#endif
