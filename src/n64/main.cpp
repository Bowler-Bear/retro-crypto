#include <libdragon.h>

#include "core_system.h"
#include "n64_display.h"

int main()
{
	debug_init_isviewer();
	debug_init_usblog();
	controller_init();

	RetroCrypto::CoreSystem& coreSystem = RetroCrypto::CoreSystem::getCoreSystem();
	std::shared_ptr<N64Display> display = std::make_shared<N64Display>();
	coreSystem.setDisplay(display);
	coreSystem.init();
	while (!coreSystem.getQuitRequested())
	{
		coreSystem.tick();
	}
	return 0;
}
