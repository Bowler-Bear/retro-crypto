#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <queue>
#include <memory>

#include "system.h"
#include "input_capturer.h"

namespace RetroCrypto
{
	class InputSystem : public ISystem
	{
	private:
		std::queue<InputType> inputQueue;

		std::shared_ptr<IInputCapturer> inputCapturer;

		InputSystem();

		~InputSystem();

		InputSystem(InputSystem const&);

		void operator=(InputSystem const&);
	public:
		static InputSystem* getInputSystem();

		virtual bool init() override;

		virtual void tick() override;

		void setInputCapturer(std::shared_ptr<IInputCapturer> capturer);

		InputType getNextInput();
	};
}
#endif
