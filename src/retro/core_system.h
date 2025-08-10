#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include "system.h"
#include "menu_system.h"
#include "input_system.h"
#include "core_context.h"
#include "random_number_generator.h"

namespace RetroCrypto
{
	class CoreSystem: public ISystem
	{
	private:
		MenuSystem* menuSystem;

		InputSystem* inputSystem;

		std::shared_ptr<IRandomNumberGenerator> rng;

		ContextData contextData;

		CoreSystem();

		~CoreSystem();

		CoreSystem(CoreSystem const&);

		void operator=(CoreSystem const&);
	public:
		static CoreSystem& getCoreSystem();

		void setDisplay(std::shared_ptr<IDisplay> inDisplay);

		void setInputCapturer(std::shared_ptr<IInputCapturer> capturer);

		void setRandomNumberGenerator(std::shared_ptr<IRandomNumberGenerator> rng);

		std::shared_ptr<IRandomNumberGenerator> getRandomNumberGenerator();

		void tick();

		virtual bool init() override;

		void updateContextData(ContextUpdate update, ContextData dataContainer);

		ContextData getContextData();
	};
}
#endif
