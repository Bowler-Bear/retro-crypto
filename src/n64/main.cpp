#include <libdragon.h>

#include "core_system.h"
int main()
{
	debug_init_isviewer();
	debug_init_usblog();
	controller_init();

	console_init();
	console_set_render_mode(RENDER_MANUAL);

	RetroCrypto::CoreSystem& coreSystem = RetroCrypto::CoreSystem::getCoreSystem();
	coreSystem.init();
	while (!coreSystem.getQuitRequested())
	{
		coreSystem.tick();
	}
	return 0;
}
