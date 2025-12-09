#ifndef N64_CAPTURER_H
#define N64_CAPTURER_H

#include <memory>

#include "input_capturer.h"
#include "n64_display.h"

using namespace RetroCrypto;

class N64Capturer: public IInputCapturer
{
private:
	std::shared_ptr<N64Display> display;
	uint64_t lastViewportMovementMS;
public:
	N64Capturer();
	virtual InputType getInput() override;
	void setDisplay(std::shared_ptr<N64Display> display);
};
#endif
