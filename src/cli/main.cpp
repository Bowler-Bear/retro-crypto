#include "cli_display.h"
#include "cli_capturer.h"
#include "core_system.h"
#include <unistd.h>

int main()
{
	RetroCrypto::CoreSystem& coreSystem = RetroCrypto::CoreSystem::getCoreSystem();
	std::shared_ptr<CLIDisplay> display = std::make_shared<CLIDisplay>();
	std::shared_ptr<CLICapturer> capturer = std::make_shared<CLICapturer>();
	coreSystem.setDisplay(display);
	coreSystem.setInputCapturer(capturer);
	coreSystem.init();
	while (!coreSystem.getQuitRequested())
	{
		coreSystem.tick();
		usleep(100000);
	}
	return 0;
}
