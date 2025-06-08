#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "display_factory.h"
#include "menu_tree.h"

namespace RetroCrypto
{
	class MenuSystem
	{
	private:
		bool quitRequested;

		std::shared_ptr<MenuTree> menuTree;
		
		std::shared_ptr<MenuTreeObject> currentMenuPosition;

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
