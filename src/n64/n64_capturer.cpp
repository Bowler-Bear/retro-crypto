#include <controller.h>
#include <n64sys.h>

#include "n64_capturer.h"


#define N64_ANALOG_MAX_VAUE 85

#ifndef N64_ANALOG_X_DEADZONE_PERCENTAGE
#define N64_ANALOG_X_DEADZONE_PERCENTAGE 20
#endif
#ifndef N64_ANALOG_Y_DEADZONE_PERCENTAGE
#define N64_ANALOG_Y_DEADZONE_PERCENTAGE 20
#endif

#ifndef N64_VIEWPORT_MAX_MOVEMENT_PER_SECOND
#define N64_VIEWPORT_MAX_MOVEMENT_PER_SECOND 2
#endif

N64Capturer::N64Capturer()
{
	controller_init();
	display = nullptr;
	lastViewportMovementMS = get_ticks_ms();
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
	if (get_ticks_ms() - lastViewportMovementMS < 1000/N64_VIEWPORT_MAX_MOVEMENT_PER_SECOND)
	{
		return InputType::NONE;
	}
	data = get_keys_held();
	for (uint8_t i = 0; i < maxControllerCount; i++)
	{
		int analogX = abs(data.c[i].x);
		int analogY = abs(data.c[i].y);
		if (display && (analogX > N64_ANALOG_MAX_VAUE*N64_ANALOG_X_DEADZONE_PERCENTAGE/100 || analogY > N64_ANALOG_MAX_VAUE*N64_ANALOG_Y_DEADZONE_PERCENTAGE/100))
		{
			lastViewportMovementMS = get_ticks_ms();
			if (analogX >= analogY)
				display->updateViewPortOffsetX(data.c[i].x > 0 ? 1 : -1);
			else
				display->updateViewPortOffsetY(data.c[i].y > 0 ? -1 : 1);
			break;
		}
	}
	return InputType::NONE;
}

void N64Capturer::setDisplay(std::shared_ptr<N64Display> inDisplay)
{
	display = inDisplay;
}
