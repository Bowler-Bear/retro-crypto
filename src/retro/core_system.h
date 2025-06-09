#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include "menu_system.h"
#include "system.h"

namespace RetroCrypto
{
	class CoreSystem: public ISystem
	{
	private:
		MenuSystem* menuSystem;

		CoreSystem();

		~CoreSystem();

		CoreSystem(CoreSystem const&);

		void operator=(CoreSystem const&);
	public:
		static CoreSystem& getCoreSystem();

		void setDisplayFactory(IDisplayFactory& factory);

		void tick();

		virtual bool init() override;
	};
}
#endif
