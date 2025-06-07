#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "menu_factory.h"

namespace RetroCrypto
{
	class MenuSystem
	{
	private:
		bool quitRequested;

		IMenuFactory* menuFactory;

		IMenu* menu;

		MenuSystem();

		~MenuSystem();

		MenuSystem(MenuSystem const&);

		void operator=(MenuSystem const&);
	public:
		static MenuSystem* getMenuSystem();

		bool getQuitRequested();

		void setQuitRequested(bool newValue);

		void setMenuFactory(IMenuFactory& factory);

		void init();

		void redraw();
	};
}
#endif
