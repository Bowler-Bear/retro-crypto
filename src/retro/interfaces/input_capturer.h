#ifndef INPUT_CAPTURER_H
#define INPUT_CAPTURER_H

namespace RetroCrypto
{
	enum InputType
	{
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		FORWARD,
		BACK,
		SHOW_NEXT_ALT_MENU,
		SHOW_PREVIOUS_ALT_MENU
	};

	class IInputCapturer
	{
	public:
		virtual InputType getInput() = 0;
	};
}
#endif
