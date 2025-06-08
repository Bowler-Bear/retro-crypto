#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "display_factory.h"

namespace RetroCrypto
{
	class MenuSystem
	{
	private:
		bool quitRequested;

		IDisplayFactory* displayFactory;

		IDisplay* display;

		MenuSystem();

		~MenuSystem();

		MenuSystem(MenuSystem const&);

		void operator=(MenuSystem const&);
	public:
		static MenuSystem* getMenuSystem();

		bool getQuitRequested();

		void setQuitRequested(bool newValue);

		void setDisplayFactory(IDisplayFactory& factory);

		void init();

		void redraw();
	};
}
#endif
