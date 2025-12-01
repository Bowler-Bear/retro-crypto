#include <controller.h>

#include "n64_capturer.h"

N64Capturer::N64Capturer()
{
	controller_init();
	display = nullptr;
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
		if (data.c[i].C_up && display)
		{
			display->increaseCharacterScale();
		}
		else if (data.c[i].C_down && display)
		{
			display->decreaseCharacterScale();
		}
		if (data.c[i].start && display)
		{
			display->resetViewPortOffsets();
		}
	}
	data = get_keys_held();
	for (uint8_t i = 0; i < maxControllerCount; i++)
	{
		if (data.c[i].x && display)
		{
			display->updateViewPortOffsetX(data.c[i].x > 0 ? 1 : -1);
		}
		else if (data.c[i].y && display)
		{
			display->updateViewPortOffsetY(data.c[i].y > 0 ? -1 : 1);
		}
	}
	return InputType::NONE;
}

void N64Capturer::setDisplay(std::shared_ptr<N64Display> inDisplay)
{
	display = inDisplay;
}
