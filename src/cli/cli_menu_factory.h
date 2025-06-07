#ifndef CLI_MENU_FACTORY_H
#define CLI_MENU_FACTORY_H

#include "cli_menu.h"
#include "menu_factory.h"

using namespace RetroCrypto;

class CLIMenuFactory: public IMenuFactory
{
public:
	virtual IMenu* constructMenu() override
	{
		return new CLIMenu();
	}
};
#endif
