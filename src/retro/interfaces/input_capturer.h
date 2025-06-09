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
		BACK
	};

	class IInputCapturer
	{
	public:
		virtual InputType getInput() = 0;
	};
}
#endif
