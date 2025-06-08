#ifndef CLI_DISPLAY_FACTORY_H
#define CLI_DISPLAY_FACTORY_H

#include "cli_display.h"
#include "display_factory.h"

using namespace RetroCrypto;

class CLIDisplayFactory: public IDisplayFactory
{
public:
	virtual IDisplay* constructDisplay() override
	{
		return new CLIDisplay();
	}
};
#endif
