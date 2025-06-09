#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include "system.h"
#include "menu_system.h"
#include "input_system.h"

namespace RetroCrypto
{
	class CoreSystem: public ISystem
	{
	private:
		MenuSystem* menuSystem;

		InputSystem* inputSystem;

		CoreSystem();

		~CoreSystem();

		CoreSystem(CoreSystem const&);

		void operator=(CoreSystem const&);
	public:
		static CoreSystem& getCoreSystem();

		void setDisplay(std::shared_ptr<IDisplay> inDisplay);

		void setInputCapturer(std::shared_ptr<IInputCapturer> capturer);

		void tick();

		virtual bool init() override;
	};
}
#endif
