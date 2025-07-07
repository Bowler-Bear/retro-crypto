#ifndef N64_CAPTURER_H
#define N64_CAPTURER_H

#include "input_capturer.h"

using namespace RetroCrypto;

class N64Capturer: public IInputCapturer
{
public:
	N64Capturer();
	virtual InputType getInput() override;
};
#endif
