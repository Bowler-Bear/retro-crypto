#include <libdragon.h>

#include "core_system.h"
#include "n64_display.h"
#include "n64_capturer.h"

uint32_t vBlankCounter = 0;

void vBlankTriggered()
{
	vBlankCounter++;
	if (vBlankCounter > N64_RESEED_EVERY_X_V_BLANKS)
	{
		std::shared_ptr<IRandomNumberGenerator> rng = RetroCrypto::CoreSystem::getCoreSystem().getRandomNumberGenerator();
		const uint8_t bufferSize = sizeof(uint64_t)/sizeof(uint8_t);
		uint8_t buffer[bufferSize] = { 0 };
		uint64_t ticks = get_ticks();
		for (uint8_t i = 0; i < bufferSize; i++)
			buffer[i] = (ticks >> ((bufferSize-1-i)*8)) & 0xff;
		rng->seedGenerator(buffer, bufferSize);
		vBlankCounter = 0;
	}
}

int main()
{
	debug_init_isviewer();

	RetroCrypto::CoreSystem& coreSystem = RetroCrypto::CoreSystem::getCoreSystem();
	std::shared_ptr<N64Display> display = std::make_shared<N64Display>();
	std::shared_ptr<UnifontHandler> unifontHandler = coreSystem.getUnifontHandler();
	unifontHandler->setFilePath("rom://reduced_unifont.hex");
	dfs_init( DFS_DEFAULT_LOCATION );
	display->setUnifontHandler(unifontHandler);
	std::shared_ptr<N64Capturer> capturer = std::make_shared<N64Capturer>();
	capturer->setDisplay(display);
	coreSystem.setDisplay(display);
	coreSystem.setInputCapturer(capturer);
	coreSystem.init();
	register_VI_handler((void(*)(void))vBlankTriggered);
	while (!coreSystem.getQuitRequested())
	{
		coreSystem.tick();
	}
	return 0;
}
