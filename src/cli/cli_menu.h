#ifndef CLI_MENU_H
#define CLI_MENU_H

#include "menu.h"

using namespace RetroCrypto;

class CLIMenu: public IMenu
{
public:
	CLIMenu();
	virtual void redraw() override;
	virtual void clear() override;
};
#endif
