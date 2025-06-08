#ifndef CLI_DISPLAY_H
#define CLI_DISPLAY_H

#include "display.h"

using namespace RetroCrypto;

class CLIDisplay: public IDisplay
{
public:
	CLIDisplay();
	virtual void redraw() override;
	virtual void clear() override;
};
#endif
