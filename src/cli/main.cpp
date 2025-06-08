#include "cli_display_factory.h"
#include "menu_system.h"
#include <unistd.h>

int main()
{
	RetroCrypto::MenuSystem* menuSystem = RetroCrypto::MenuSystem::getMenuSystem();
	CLIDisplayFactory CLIDisplayFactory;
	menuSystem->setDisplayFactory(CLIDisplayFactory);
	menuSystem->init();
	while (!menuSystem->getQuitRequested())
	{
		menuSystem->redraw();
		sleep(1);
	}
	return 0;
}
