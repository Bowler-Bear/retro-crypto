#include <libdragon.h>

#include "core_system.h"
#include "n64_display.h"
#include "n64_capturer.h"

int main()
{
	debug_init_isviewer();
	debug_init_usblog();

	RetroCrypto::CoreSystem& coreSystem = RetroCrypto::CoreSystem::getCoreSystem();
	std::shared_ptr<N64Display> display = std::make_shared<N64Display>();
	std::shared_ptr<N64Capturer> capturer = std::make_shared<N64Capturer>();
	coreSystem.setDisplay(display);
	coreSystem.setInputCapturer(capturer);
	coreSystem.init();
	srand(get_ticks());
	register_VI_handler((void(*)(void))rand);
	while (!coreSystem.getQuitRequested())
	{
		controller_scan();
		coreSystem.tick();
	}
	return 0;
}
