#include <libdragon.h>

#include "core_system.h"
#include "n64_display.h"
#include "n64_capturer.h"

int main()
{
	debug_init_isviewer();

	RetroCrypto::CoreSystem& coreSystem = RetroCrypto::CoreSystem::getCoreSystem();
	std::shared_ptr<N64Display> display = std::make_shared<N64Display>();
	std::shared_ptr<N64Capturer> capturer = std::make_shared<N64Capturer>();
	capturer->setDisplay(display);
	coreSystem.setDisplay(display);
	coreSystem.setInputCapturer(capturer);
	coreSystem.init();
	srand(get_ticks());
	register_VI_handler((void(*)(void))rand);
	while (!coreSystem.getQuitRequested())
	{
		coreSystem.tick();
	}
	return 0;
}
