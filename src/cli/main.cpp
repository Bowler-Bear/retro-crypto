#include "cli_display_factory.h"
#include "cli_capturer.h"
#include "core_system.h"
#include <unistd.h>

int main()
{
	RetroCrypto::CoreSystem& coreSystem = RetroCrypto::CoreSystem::getCoreSystem();
	CLIDisplayFactory CLIDisplayFactory;
	std::shared_ptr<CLICapturer> capturer = std::make_shared<CLICapturer>();
	coreSystem.setDisplayFactory(CLIDisplayFactory);
	coreSystem.setInputCapturer(capturer);
	coreSystem.init();
	while (!coreSystem.getQuitRequested())
	{
		coreSystem.tick();
		usleep(100000);
	}
	return 0;
}
