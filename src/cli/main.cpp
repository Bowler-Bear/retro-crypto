#include "cli_menu_factory.h"
#include "menu_system.h"
#include <unistd.h>

int main()
{
	RetroCrypto::MenuSystem* menuSystem = RetroCrypto::MenuSystem::getMenuSystem();
	CLIMenuFactory cliMenuFactory;
	menuSystem->setMenuFactory(cliMenuFactory);
	menuSystem->init();
	while (!menuSystem->getQuitRequested())
	{
		menuSystem->redraw();
		sleep(1);
	}
	return 0;
}
