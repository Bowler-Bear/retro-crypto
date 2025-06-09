#include "cli_display_factory.h"
#include "core_system.h"
#include <unistd.h>

int main()
{
	RetroCrypto::CoreSystem& coreSystem = RetroCrypto::CoreSystem::getCoreSystem();
	CLIDisplayFactory CLIDisplayFactory;
	coreSystem.setDisplayFactory(CLIDisplayFactory);
	coreSystem.init();
	while (!coreSystem.getQuitRequested())
	{
		coreSystem.tick();
		sleep(1);
	}
	return 0;
}
