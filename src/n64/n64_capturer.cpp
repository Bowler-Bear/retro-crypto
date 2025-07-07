#include <controller.h>

#include "n64_capturer.h"

N64Capturer::N64Capturer()
{
	controller_init();
}

InputType N64Capturer::getInput()
{
	controller_scan();
	struct controller_data data = get_keys_down();
	const uint8_t maxControllerCount = 4;
	for (uint8_t i = 0; i < maxControllerCount; i++)
	{
		if (data.c[i].down)
		{
			return InputType::DOWN;
		}
		else if (data.c[i].up)
		{
			return InputType::UP;
		}
		else if (data.c[i].A)
		{
			return InputType::FORWARD;
		}
		else if (data.c[i].B)
		{
			return InputType::BACK;
		}
		else if (data.c[i].left)
		{
			return InputType::LEFT;
		}
		else if (data.c[i].right)
		{
			return InputType::RIGHT;
		}
		
	}
	return InputType::NONE;
}
