#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "system.h"

namespace RetroCrypto
{
	class InputSystem : public ISystem
	{
	private:
		InputSystem();

		~InputSystem();

		InputSystem(InputSystem const&);

		void operator=(InputSystem const&);
	public:
		static InputSystem* getInputSystem();

		virtual bool init() override;

		virtual void tick() override;
	};
}
#endif
