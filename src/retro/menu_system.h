#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "system.h"
#include "retro_display.h"
#include "menu_tree.h"
#include "input_capturer.h"

namespace RetroCrypto
{
	class MenuSystem : public ISystem
	{
	private:
		MenuTree menuTree;
		
		std::shared_ptr<MenuTreeObject> currentMenuPosition;

		std::shared_ptr<IDisplay> display;

		MenuSystem();

		~MenuSystem();

		MenuSystem(MenuSystem const&);

		void operator=(MenuSystem const&);
	public:
		static MenuSystem* getMenuSystem();

		void setDisplay(std::shared_ptr<IDisplay> inDisplay);

		virtual bool init() override;

		virtual void tick() override;

		void redraw();

		void processInput(InputType input);
	};
}
#endif
